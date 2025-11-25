/*
 * A reimplementation of Jim Dose's FX_MAN routines, using  SDL_mixer 1.2.
 *   Whee. FX_MAN is also known as the "Apogee Sound System", or "ASS" for
 *   short. How strangely appropriate that seems.
 *
 * Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */

#include "SDL.h"
#include "SDL_mixer.h"

#if defined(HAVE_ADLMIDI)
#include <adlmidi.h>
#endif

#include "i_glob.h"

#include "rt_def.h"	 // ROTT music hack
#include "rt_cfg.h"	 // ROTT music hack
#include "rt_util.h" // ROTT music hack

#include "music.h"

#define __FX_TRUE (1 == 1)
#define __FX_FALSE (!__FX_TRUE)

static int music_initialized = 0;
static int music_context = 0;
static int music_loopflag = MUSIC_PlayOnce;
static unsigned char *music_songdata = NULL;
static Mix_Music *music_musicchunk = NULL;
static size_t music_songdatasize = 0;

char *soundfont_cfg = "soundfont.sf2";

#if !defined(_WIN32)
char *UserHomeDir(void)
{
	static char *home_dir;

	if (home_dir == NULL)
	{
		home_dir = M_getenv("HOME");

		if (home_dir == NULL)
		{
			home_dir = "/";
		}
	}

	return home_dir;
}

char *UserDataDir(void)
{
	static char *data_dir;

	if (data_dir == NULL)
	{
		data_dir = M_getenv("XDG_DATA_HOME");

		if (data_dir == NULL || *data_dir == '\0')
		{
			const char *home_dir = UserHomeDir();
			data_dir = M_StringJoin(home_dir, "/.local/share", NULL);
		}
	}

	return data_dir;
}

static const char *ScanDir(const char *dir)
{
	const char *ret = NULL;

	glob_t *glob = I_StartMultiGlob(dir, GLOB_FLAG_NOCASE | GLOB_FLAG_SORTED,
									"*.sf2", "*.sf3", NULL);

	const char *filename = I_NextGlob(glob);
	if (filename)
	{
		ret = M_StringDuplicate(filename);
	}

	I_EndGlob(glob);

	return ret;
}

static const char *GetSoundFont(void)
{
	const char *ret = NULL;

	const struct
	{
		char *(*func)(void);
		const char *dir;
	} dirs[] = {
		// RedHat/Fedora/Arch
		{ NULL, "/usr/share/soundfonts" },
		{ UserDataDir, "soundfonts" },
		// Debian/Ubuntu/OpenSUSE
		{ NULL, "/usr/share/sounds/sf2" },
		{ UserDataDir, "sounds/sf2" },
		{ NULL, "/usr/share/sounds/sf3" },
		{ UserDataDir, "sounds/sf3" },
	};

	for (int i = 0; i < arrlen(dirs); ++i)
	{
		if (dirs[i].func && dirs[i].dir)
		{
			char *dir = M_StringJoin(dirs[i].func(), DIR_SEPARATOR_S,
									 dirs[i].dir, NULL);
			ret = ScanDir(dir);
			free(dir);
		}
		else if (dirs[i].func)
		{
			ret = ScanDir(dirs[i].func());
		}
		else if (dirs[i].dir)
		{
			ret = ScanDir(dirs[i].dir);
		}

		if (ret)
		{
			break;
		}
	}

	return ret;
}
#endif

static Uint32 is_playing = 0;
#if defined(HAVE_ADLMIDI)
static struct ADL_MIDIPlayer *midi_player = NULL;

void adlmidi_callback(void *dummy, Uint8 *stream, int len)
{
	if (!is_playing)
	{
		return;
	}

	/* Convert bytes length into total count of samples in all channels */
	int samples_count = len / sizeof(short);

	/* Take some samples from the ADLMIDI */
	samples_count = adl_play(midi_player, samples_count, (short *)stream);

	if (samples_count <= 0)
	{
		if (music_loopflag == MUSIC_PlayOnce)
		{
			is_playing = 0;
			SDL_memset(stream, 0, len);
		}
		else
		{
			adl_positionRewind(midi_player);
		}
	}
}
#endif

int MUSIC_Init(int SoundCard, int Address)
{
#if !defined(HAVE_ADLMIDI)
	const char *soundfont = M_FileCaseExists(soundfont_cfg);
	if (soundfont == NULL)
	{
		soundfont = Mix_GetSoundFonts();
	}

#if !defined(_WIN32)
	if (soundfont == NULL)
	{
		soundfont = GetSoundFont();
	}
#endif

	if (soundfont)
	{
		printf("\n Soundfont: %s", soundfont);
		Mix_SetSoundFonts(soundfont);
	}
	else
	{
		printf("\n No Soundfont found!");
	}
#else
	int snd_samplerate;
	if (Mix_QuerySpec(&snd_samplerate, NULL, NULL) == 0)
	{
		fprintf(stderr, "SDL_Mixerneeds to be initialized first!\n");
		return (MUSIC_Error);
	}

	midi_player = adl_init(snd_samplerate);
	if (!midi_player)
	{
		fprintf(stderr, "Couldn't initialize ADLMIDI: %s\n", adl_errorString());
		return (MUSIC_Error);
	}
	Mix_HookMusic(adlmidi_callback, NULL);
#endif

	music_initialized = 1;
	return (MUSIC_Ok);
}

int MUSIC_Shutdown(void)
{
	MUSIC_StopSong();
	music_context = 0;
	music_initialized = 0;
	music_loopflag = MUSIC_PlayOnce;
	is_playing = 0;

	return (MUSIC_Ok);
}

void MUSIC_SetVolume(int volume)
{
	Mix_VolumeMusic(volume >> 1); // convert 0-255 to 0-128.
}

int MUSIC_GetVolume(void)
{
	return (Mix_VolumeMusic(-1) << 1); // convert 0-128 to 0-255.
}

int MUSIC_SongPlaying(void)
{
#if defined(HAVE_ADLMIDI)
	return is_playing;
#else
	return ((Mix_PlayingMusic()) ? __FX_TRUE : __FX_FALSE);
#endif
}

void MUSIC_Continue(void)
{
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
	else if (music_songdata)
		MUSIC_PlaySong(music_songdata, music_songdatasize, MUSIC_PlayOnce);
}

void MUSIC_Pause(void)
{
	Mix_PauseMusic();
}

int MUSIC_StopSong(void)
{
	if ((Mix_PlayingMusic()) || (Mix_PausedMusic()))
		Mix_HaltMusic();

	if (music_musicchunk)
		Mix_FreeMusic(music_musicchunk);

	music_songdata = NULL;
	music_musicchunk = NULL;
	music_songdatasize = 0;
	is_playing = 0;

	return (MUSIC_Ok);
}

int MUSIC_PlaySong(unsigned char *song, int size, int loopflag)
{
	MUSIC_StopSong();

	if (size < 1)
	{
		return MUSIC_Error;
	}

#if defined(HAVE_ADLMIDI)
	if (adl_openData(midi_player, song, size) < 0)
	{
		fprintf(stderr, "Couldn't open music file: %s\n",
				adl_errorInfo(midi_player));
		return MUSIC_Error;
	}
#else
	// create rw
	SDL_RWops *rw = SDL_RWFromConstMem(song, size);
	if (rw == NULL)
	{
		return MUSIC_Error;
	}

	// load with SDL_mixer
	music_musicchunk = Mix_LoadMUS_RW(rw, SDL_TRUE);
	if (music_musicchunk == NULL)
	{
		return MUSIC_Error;
	}

	Mix_PlayMusic(music_musicchunk, (loopflag == MUSIC_PlayOnce) ? 0 : -1);
#endif

	music_songdata = song;
	music_songdatasize = size;
	music_loopflag = loopflag;

	is_playing = 1;

	return (MUSIC_Ok);
}

void MUSIC_SetSongTime(unsigned long milliseconds)
{
	(void)milliseconds;
}

void MUSIC_GetSongPosition(songposition *pos)
{
	(void)pos;
}

int MUSIC_FadeVolume(int tovolume, int milliseconds)
{
	Mix_FadeOutMusic(milliseconds);
	return (MUSIC_Ok);
}

int MUSIC_FadeActive(void)
{
#if defined(HAVE_ADLMIDI)
	return __FX_FALSE;
#else
	return ((Mix_FadingMusic() == MIX_FADING_OUT) ? __FX_TRUE : __FX_FALSE);
#endif
}

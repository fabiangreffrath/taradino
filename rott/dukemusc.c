/*
 * A reimplementation of Jim Dose's FX_MAN routines, using  SDL_mixer 1.2.
 *   Whee. FX_MAN is also known as the "Apogee Sound System", or "ASS" for
 *   short. How strangely appropriate that seems.
 *
 * Written by Ryan C. Gordon. (icculus@clutteredmind.org)
 */

#include "SDL_mixer.h"

#include "rt_util.h"
#include "music.h"

static music_module_t *music_modules[] = { &dummy_music_module,
										   &sdl_music_module,
#if defined(HAVE_ADLMIDI)
										   &adl_music_module
#endif
};

static music_module_t *music_module = &dummy_music_module;
const int num_music_modules = arrlen(music_modules);

unsigned char *music_songdata = NULL;
size_t music_songdatasize = 0;
int music_loopflag = MUSIC_PlayOnce;
double float_music_volume;

int MUSIC_Init(int mode)
{
	int snd_samplerate;
	if (Mix_QuerySpec(&snd_samplerate, NULL, NULL) == 0)
	{
		Error("SDL2_Mixer needs to be initialized first!\n");
	}

	music_module->Shutdown();

	music_module = music_modules[mode];

	return music_module->Init(snd_samplerate);
}

int MUSIC_Shutdown(void)
{
	music_loopflag = MUSIC_PlayOnce;

	return music_module->Shutdown();
}

void MUSIC_SetVolume(int volume)
{
	float_music_volume = 10.0 * pow(volume / 255.0, 2.0);
	Mix_VolumeMusic(volume >> 1); // convert 0-255 to 0-128.
}

int MUSIC_SongPlaying(void)
{
	return music_module->SongPlaying();
}

void MUSIC_Continue(void)
{
	music_module->Continue();
}

void MUSIC_Pause(void)
{
	music_module->Pause();
}

int MUSIC_StopSong(void)
{
	music_songdata = NULL;
	music_songdatasize = 0;

	return music_module->StopSong();
}

int MUSIC_PlaySong(unsigned char *song, int size, int loopflag)
{
	MUSIC_StopSong();

	if (size < 1)
	{
		return MUSIC_Error;
	}

	music_songdata = song;
	music_songdatasize = size;
	music_loopflag = loopflag;

	return music_module->PlaySong(song, size, loopflag);
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
	return music_module->FadeVolume(tovolume, milliseconds);
}

int MUSIC_FadeActive(void)
{
	return music_module->FadeActive();
}

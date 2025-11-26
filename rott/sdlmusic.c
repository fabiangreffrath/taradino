/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2025 Fabian Greffrath

 This package is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 .
 This package is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 .
 You should have received a copy of the GNU General Public License
 along with this package. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SDL_mixer.h"

#include "i_glob.h"
#include "m_misc.h"

#include "music.h"

static Mix_Music *music_musicchunk = NULL;

char *soundfont_cfg = "soundfont.sf2";

#if !defined(_WIN32)
static const char *UserHomeDir(void)
{
	const char *home_dir = M_getenv("HOME");

	if (home_dir == NULL)
	{
		home_dir = "/";
	}

	return home_dir;
}

static const char *UserDataDir(void)
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
		const char *(*func)(void);
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

static int sdl_inited = 0;

int SDLmusic_Init(int dummy)
{
	(void)dummy;

	if (sdl_inited == 0)
	{
		const char *mix_soundfont = Mix_GetSoundFonts();
		const char *soundfont = mix_soundfont;

		if (soundfont == NULL)
		{
			soundfont = M_FileCaseExists(soundfont_cfg);
		}

#if !defined(_WIN32)
		if (soundfont == NULL)
		{
			soundfont = GetSoundFont();
		}
#endif

		if (soundfont)
		{
			if (!mix_soundfont)
			{
				Mix_SetSoundFonts(soundfont);
			}
			printf(" Using Soundfont %s\n", soundfont);
		}
		else
		{
			printf(" No Soundfont found!\n");
		}

		sdl_inited = 1;
	}

	Mix_HookMusic(NULL, NULL);

	return MUSIC_Ok;
}

static int SDLmusic_SongPlaying(void)
{
	return (Mix_PlayingMusic() ? __FX_TRUE : __FX_FALSE);
}

static void SDLmusic_Pause(void)
{
	Mix_PauseMusic();
}

static int SDLmusic_StopSong(void)
{
	if (Mix_PlayingMusic() || Mix_PausedMusic())
		Mix_HaltMusic();

	if (music_musicchunk)
		Mix_FreeMusic(music_musicchunk);

	music_musicchunk = NULL;

	return MUSIC_Ok;
}

static int SDLmusic_Shutdown(void)
{
	SDLmusic_StopSong();

	return MUSIC_Ok;
}

static int SDLmusic_PlaySong(unsigned char *song, int size, int loopflag)
{
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

	return MUSIC_Ok;
}

static void SDLmusic_Continue(void)
{
	if (Mix_PausedMusic())
		Mix_ResumeMusic();
	else if (music_songdata)
		SDLmusic_PlaySong(music_songdata, music_songdatasize, MUSIC_PlayOnce);
}

static int SDLmusic_FadeVolume(int tovolume, int milliseconds)
{
	Mix_FadeOutMusic(milliseconds);
	return MUSIC_Ok;
}

static int SDLmusic_FadeActive(void)
{
	return (Mix_FadingMusic() == MIX_FADING_OUT) ? __FX_TRUE : __FX_FALSE;
}

music_module_t sdl_music_module = {
	SDLmusic_Init,	   SDLmusic_Shutdown,	SDLmusic_SongPlaying,
	SDLmusic_Continue, SDLmusic_Pause,		SDLmusic_StopSong,
	SDLmusic_PlaySong, SDLmusic_FadeVolume, SDLmusic_FadeActive,
};

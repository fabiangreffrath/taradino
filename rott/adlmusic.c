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

#if defined(HAVE_ADLMIDI)

#include "SDL_mixer.h"

#include <adlmidi.h>

#include "music.h"

static int is_playing = 0;
static struct ADL_MIDIPlayer *midi_player = NULL;

static void adlmidi_callback(void *dummy, Uint8 *stream, int len)
{
	if (!is_playing)
	{
		return;
	}

	/* Convert bytes length into total count of samples in all channels */
	int samples_count = len / sizeof(short);

	/* Take some samples from ADLMIDI */
	short *const stream16 = (short *)stream;
	samples_count = adl_play(midi_player, samples_count, stream16);

	for (int i = 0; i < samples_count; i++)
	{
		int32_t sample = (int32_t)(stream16[i] * float_music_volume);

		if (sample > SHRT_MAX)
			sample = SHRT_MAX;
		else if (sample < SHRT_MIN)
			sample = SHRT_MIN;

		stream16[i] = (short)sample;
	}

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

static int adl_inited = 0;

static int ADL_Init(int samplerate)
{
	if (adl_inited == 0)
	{
		midi_player = adl_init(samplerate);
		if (!midi_player)
		{
			fprintf(stderr, "Couldn't initialize ADLMIDI: %s\n",
					adl_errorString());
			return MUSIC_Error;
		}
		printf(" Using libADLMIDI %s\n", adl_linkedLibraryVersion());

		adl_inited = 1;
	}

	Mix_HookMusic(adlmidi_callback, NULL);

	return MUSIC_Ok;
}

static int ADL_SongPlaying(void)
{
	return is_playing;
}

static void ADL_Pause(void)
{
	is_playing = 0;
}

static int ADL_StopSong(void)
{
	is_playing = 0;

	return MUSIC_Ok;
}

static int ADL_Shutdown(void)
{
	return ADL_StopSong();
}

static int ADL_PlaySong(unsigned char *song, int size, int loopflag)
{
	if (adl_openData(midi_player, song, size) < 0)
	{
		fprintf(stderr, "Couldn't open music file: %s\n",
				adl_errorInfo(midi_player));
		return MUSIC_Error;
	}

	is_playing = 1;

	return MUSIC_Ok;
}

static void ADL_Continue(void)
{
	if (music_songdata)
		ADL_PlaySong(music_songdata, music_songdatasize, MUSIC_PlayOnce);
}

static int ADL_FadeVolume(int tovolume, int milliseconds)
{
	(void)tovolume;
	(void)milliseconds;
	return MUSIC_Ok;
}

static int ADL_FadeActive(void)
{
	return __FX_FALSE;
}

music_module_t adl_music_module = {
	ADL_Init,	  ADL_Shutdown, ADL_SongPlaying, ADL_Continue,	 ADL_Pause,
	ADL_StopSong, ADL_PlaySong, ADL_FadeVolume,	 ADL_FadeActive,
};

#endif

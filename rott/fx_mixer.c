/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2023 Fabian Greffrath

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "SDL.h"
#include "SDL_mixer.h"

#include "rt_def.h"
#include "w_wad.h"
#include "z_zone.h"

#include "fx_man.h"
#include "_rt_soun.h"
#include "rt_sound.h"

#if (SHAREWARE == 0)
#include "snd_reg.h"
#else
#include "snd_shar.h"
#endif

#define MAX_CHANNELS 8

static struct
{
    sound_t *sfx;
} channels[MAX_CHANNELS];

static int FX_Installed = 0;

/**********************************************************************
   module: MULTIVOC.C

   author: James R. Dose
   date:   December 20, 1993

   Routines to provide multichannel digitized sound playback for
   Sound Blaster compatible sound cards.

   (c) Copyright 1993 James R. Dose.  All Rights Reserved.
**********************************************************************/

#define MV_MaxPanPosition  31
#define MV_NumPanPositions (MV_MaxPanPosition + 1)

#define MIX_VOLUME(volume)                                                     \
    ((max(0, min((volume), 255)) * (MV_MaxVolume + 1)) >> 8)

typedef struct
{
    uint8_t left;
    uint8_t right;
} Pan;

static Pan MV_PanTable[MV_NumPanPositions][63 + 1];
static int MV_MaxVolume = 63;

/*---------------------------------------------------------------------
   Function: MV_CalcPanTable

   Create the table used to determine the stereo volume level of
   a sound located at a specific angle and distance from the listener.
---------------------------------------------------------------------*/

static void MV_CalcPanTable(void)
{
    int level;
    int angle;
    int distance;
    int HalfAngle;
    int ramp;

    HalfAngle = (MV_NumPanPositions / 2);

    for (distance = 0; distance <= MV_MaxVolume; distance++)
    {
        level = (255 * (MV_MaxVolume - distance)) / MV_MaxVolume;
        for (angle = 0; angle <= HalfAngle / 2; angle++)
        {
            ramp = level - ((level * angle) / (MV_NumPanPositions / 4));

            MV_PanTable[angle][distance].left = ramp;
            MV_PanTable[HalfAngle - angle][distance].left = ramp;
            MV_PanTable[HalfAngle + angle][distance].left = level;
            MV_PanTable[MV_MaxPanPosition - angle][distance].left = level;

            MV_PanTable[angle][distance].right = level;
            MV_PanTable[HalfAngle - angle][distance].right = level;
            MV_PanTable[HalfAngle + angle][distance].right = ramp;
            MV_PanTable[MV_MaxPanPosition - angle][distance].right = ramp;
        }
    }
}

/*---------------------------------------------------------------------
   Function: MV_SetPan

   Sets the stereo and mono volume level of the voice associated
   with the specified handle.
---------------------------------------------------------------------*/

static int MV_SetPan(int handle, int vol, int left, int right)
{
    Mix_SetPanning(handle, left, right);

    return FX_Ok;
}

/*---------------------------------------------------------------------
   Function: MV_Pan3D

   Set the angle and distance from the listener of the voice associated
   with the specified handle.
---------------------------------------------------------------------*/

static int MV_Pan3D(int handle, int angle, int distance)
{
    int left;
    int right;
    int mid;
    int volume;

    if (distance < 0)
    {
        distance = -distance;
        angle += MV_NumPanPositions / 2;
    }

    volume = MIX_VOLUME(distance);

    // Ensure angle is within 0 - 31
    angle &= MV_MaxPanPosition;

    left = MV_PanTable[angle][volume].left;
    right = MV_PanTable[angle][volume].right;
    mid = max(0, 255 - distance);

    return MV_SetPan(handle, mid, left, right);
}

/**********************************************************************
   module: FX_MAN.C

   author: James R. Dose
   date:   March 17, 1994

   Device independant sound effect routines.

   (c) Copyright 1994 James R. Dose.  All Rights Reserved.
**********************************************************************/

void FX_SetReverb(int reverb)
{
}

void FX_SetVolume(int volume)
{
#if SDL_MIXER_VERSION_ATLEAST(2, 6, 0)
    Mix_MasterVolume(volume >> 1);
#else
    int i;

    for (i = 0; i < MAX_CHANNELS; i++)
    {
        Mix_Volume(i, volume >> 1);
    }
#endif
}

char *FX_ErrorString(int ErrorNumber)
{
    return (char *) SDL_GetError();
}

int FX_StopSound(int handle)
{
    if (handle >= 0 && channels[handle].sfx)
    {
        Mix_HaltChannel(handle);

        if (channels[handle].sfx->count)
        {
            channels[handle].sfx->count--;
        }

        channels[handle].sfx = NULL;
    }

    return FX_Ok;
}

// Calculate slice size, the result must be a power of two.

static int snd_samplerate = 44100;

static int GetSliceSize(void)
{
    int limit;
    int n;

    limit = snd_samplerate / 35; // VBLCOUNTER

    // Try all powers of two, not exceeding the limit.

    for (n = 0;; ++n)
    {
        // 2^n <= limit < 2^n+1 ?

        if ((1 << (n + 1)) > limit)
        {
            return (1 << n);
        }
    }

    // Should never happen?

    return 1024;
}

int FX_SetupCard(int SoundCard, fx_device *device)
{
    Uint16 mix_format;
    int mix_channels;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "\n Couldn't initialize SDL audio: %s", SDL_GetError());
        return FX_Error;
    }

    if (Mix_OpenAudioDevice(snd_samplerate, AUDIO_S16SYS, 2, GetSliceSize(),
                            NULL, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE) < 0)
    {
        fprintf(stderr, "\n Couldn't open audio with desired format.");
        return FX_Error;
    }

    // [FG] feed actual sample frequency back into config variable
    Mix_QuerySpec(&snd_samplerate, &mix_format, &mix_channels);

    printf("\n Configured audio device with %.1f kHz (%s%d%s), %d channels.",
           (float) snd_samplerate / 1000,
           SDL_AUDIO_ISFLOAT(mix_format)    ? "F"
           : SDL_AUDIO_ISSIGNED(mix_format) ? "S"
                                            : "U",
           (int) SDL_AUDIO_BITSIZE(mix_format),
           SDL_AUDIO_BITSIZE(mix_format) > 8
               ? (SDL_AUDIO_ISBIGENDIAN(mix_format) ? "MSB" : "LSB")
               : "",
           mix_channels);

    // [FG] let SDL_Mixer do the actual sound mixing
    Mix_AllocateChannels(MAX_CHANNELS);

    FX_Installed = true;

    return FX_Ok;
}

int FX_Shutdown(void)
{
    if (FX_Installed)
    {
        Mix_CloseAudio();
        FX_Installed = 0;
    }

    return FX_Ok;
}

extern int SoundNumber(int x);

int FX_Init(int SoundCard, int numvoices, int numchannels, int samplebits,
            unsigned mixrate)
{
    int i;

    printf("\n Precaching all sound effects... ");
    for (i = 0; i < SD_LASTSOUND; i++)
    {
        int snd = SoundNumber(i);

        if (snd >= 0)
        {
            char *data;
            int size;
            SDL_RWops *rw;

            data = W_CacheLumpNum(snd, PU_STATIC, CvtNull, 1);
            size = W_LumpLength(snd);

            rw = SDL_RWFromMem(data, size);

            if (!(sounds[i].chunk = Mix_LoadWAV_RW(rw, 1)))
            {
                fprintf(stderr, "FX_Init: %s (%s)\n", SDL_GetError(),
                        W_GetNameForNum(snd));
            }

            Z_Free(data);
        }
    }
    printf("done.");

    printf("\n Calculating stereo panning... ");
    MV_CalcPanTable();
    printf("done.");

    return FX_Ok;
}

int FX_SetCallBack(void (*function)(unsigned long))
{
    return FX_Ok;
}

static int reverse_stereo;

void FX_SetReverseStereo(int setting)
{
    reverse_stereo = setting;
}

int FX_GetReverseStereo(void)
{
    return reverse_stereo;
}

int FX_SoundActive(int handle)
{
    return Mix_Playing(handle);
}

int FX_VoiceAvailable(int priority)
{
    int i;
    int min_prio = 255, min_chan = -1;

    for (i = 0; i < MAX_CHANNELS; i++)
    {
        if (!channels[i].sfx || !FX_SoundActive(i))
        {
            return i;
        }

        if (channels[i].sfx->priority < min_prio)
        {
            min_prio = channels[i].sfx->priority;
            min_chan = i;
        }
    }

    if (priority > min_prio)
    {
        return min_chan;
    }

    return -1;
}

int FX_Play(int handle, int sndnum, int pitchoffset, int angle, int distance,
            int priority)
{
    FX_StopSound(handle);

    if (sounds[sndnum].chunk)
    {
        sounds[sndnum].count++;
        channels[handle].sfx = &sounds[sndnum];

        Mix_PlayChannelTimed(handle, sounds[sndnum].chunk, 0, -1);
        FX_Pan3D(handle, angle, distance);

        return handle;
    }

    return -1;
}

int FX_SetPitch(int handle, int pitchoffset)
{
    return FX_Ok;
}

int FX_Pan3D(int handle, int angle, int distance)
{
    return MV_Pan3D(handle, angle, distance);
}

int FX_SetPan(int handle, int vol, int left, int right)
{
    return MV_SetPan(handle, vol, left, right);
}

int FX_StopAllSounds(void)
{
    int i;

    for (i = 0; i < MAX_CHANNELS; i++)
    {
        FX_StopSound(i);
    }

    return FX_Ok;
}

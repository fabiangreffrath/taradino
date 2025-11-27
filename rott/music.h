/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

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
/**********************************************************************
   module: MUSIC.H

   author: James R. Dose
   date:   March 25, 1994

   Public header for MUSIC.C

   (c) Copyright 1994 James R. Dose.  All Rights Reserved.
**********************************************************************/

#ifndef __MUSIC_H
#define __MUSIC_H

#include <stddef.h>

enum
{
	__FX_FALSE,
	__FX_TRUE
};

enum MUSIC_ERRORS
{
	MUSIC_Warning = -2,
	MUSIC_Error = -1,
	MUSIC_Ok = 0,
	MUSIC_ASSVersion,
	MUSIC_SoundCardError,
	MUSIC_MPU401Error,
	MUSIC_InvalidCard,
	MUSIC_MidiError,
	MUSIC_TaskManError,
	MUSIC_FMNotDetected,
	MUSIC_DPMI_Error
};

typedef struct
{
	unsigned long tickposition;
	unsigned long milliseconds;
	unsigned int measure;
	unsigned int beat;
	unsigned int tick;
} songposition;

enum
{
	MUSIC_PlayOnce,
	MUSIC_LoopSong
};

typedef struct
{
	int (*Init)(int samplerate);
	int (*Shutdown)(void);
	int (*SongPlaying)(void);
	void (*Continue)(void);
	void (*Pause)(void);
	int (*StopSong)(void);
	int (*PlaySong)(unsigned char *song, int size, int loopflag);
	int (*FadeVolume)(int tovolume, int milliseconds);
	int (*FadeActive)(void);
} music_module_t;

extern music_module_t sdl_music_module;
extern music_module_t adl_music_module;
extern const int num_music_modules;

extern unsigned char *music_songdata;
extern size_t music_songdatasize;
extern int music_loopflag;
extern double float_music_volume;

extern char *soundfont_cfg;

int MUSIC_Init(int mode);
int MUSIC_Shutdown(void);
void MUSIC_SetVolume(int volume);
int MUSIC_SongPlaying(void);
void MUSIC_Continue(void);
void MUSIC_Pause(void);
int MUSIC_StopSong(void);
int MUSIC_PlaySong(unsigned char *song, int size, int loopflag);

void MUSIC_SetSongTime(unsigned long milliseconds);
void MUSIC_GetSongPosition(songposition *pos);
int MUSIC_FadeVolume(int tovolume, int milliseconds);
int MUSIC_FadeActive(void);

#endif

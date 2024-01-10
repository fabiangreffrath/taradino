
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
/**********************************************************************
   module: FX_MAN.H

   author: James R. Dose
   date:   March 17, 1994

   Public header for FX_MAN.C

   (c) Copyright 1994 James R. Dose.  All Rights Reserved.
**********************************************************************/

#ifndef __FX_MAN_H
#define __FX_MAN_H

#include "sndcards.h"

typedef struct
   {
   int MaxVoices;
   int MaxSampleBits;
   int MaxChannels;
   } fx_device;

#define MonoFx   1
#define StereoFx 2

enum FX_ERRORS
   {
   FX_Warning = -2,
   FX_Error = -1,
   FX_Ok = 0,
   FX_ASSVersion,
   FX_BlasterError,
   FX_SoundCardError,
   FX_InvalidCard,
   FX_MultiVocError,
   FX_DPMI_Error
   };

char *FX_ErrorString( int ErrorNumber );
int   FX_SetupCard( int SoundCard, fx_device *device );
int   FX_Init( int SoundCard, int numvoices, int numchannels, int samplebits, uint32_t mixrate );
int   FX_Shutdown( void );
int   FX_SetCallBack( void ( *function )( unsigned long ) );
void  FX_SetVolume( int volume );

void  FX_SetReverseStereo( int setting );
int   FX_GetReverseStereo( void );
void  FX_SetReverb( int reverb );

int FX_VoiceAvailable( int priority );
int FX_SetPan( int handle, int vol, int left, int right );
int FX_SetPitch( int handle, int pitchoffset );

int FX_Play ( int handle, int sndnum, int pitchoffset, int angle, int distance, int priority );

int FX_Pan3D( int handle, int angle, int distance );
int FX_SoundActive( int handle );
int FX_StopSound( int handle );
int FX_StopAllSounds( void );

#if 0
int FX_StartDemandFeedPlayback( void ( *function )( char **ptr, unsigned long *length ),
       int rate, int pitchoffset, int vol, int left, int right,
       int priority, unsigned long callbackval );
int  FX_StartRecording( int MixRate, void ( *function )( char *ptr, int length ) );
void FX_StopRecord( void );
#endif

#endif

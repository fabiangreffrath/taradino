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

#include <stdint.h>
#include "rt_def.h"
#include "rt_util.h"
#include "rt_sound.h"
#include "rt_net.h"
#include "rt_dmand.h"
#include "_rt_dman.h"
#include "fx_man.h"
#include "develop.h"

static bool8_t Recording=FALSE;
static bool8_t Feeder=FALSE;
static uint8_t * RecordingBuffer;
static int Playingvoice;
static int RecordingPointer;
static int FeederPointer;
static bool8_t Playback=FALSE;
static bool8_t Playing=FALSE;
static char * PlaybackBuffer;
static int PlaybackPointer;
static int PlayingPointer;
static bool8_t RecordingSemaphore=FALSE;

//#define FX_StartDemandFeedPlayback MV_StartDemandFeedPlayback
//#define FX_StartRecording          MV_StartRecording
//#define FX_StopRecord              MV_StopRecord
//#include "multivoc.h"

//***************************************************************************
//
// SD_UpdatePlaybackSound - Update playback of a sound in chunks
//
//***************************************************************************
void SD_UpdatePlaybackSound ( char ** ptr, unsigned long * length )
   {
   if ( Playing==FALSE )
      {
      *ptr = NULL;
      *length = 0;
      return;
      }
   if (PlayingPointer==PlaybackPointer)
      {
      *ptr = NULL;
      *length = 0;
      if (Playback==FALSE)
         {
         FX_StopSound( Playingvoice );
         SafeFree ( PlaybackBuffer );
         Playing=FALSE;
         }
      return;
      }

   *length=PLAYBACKDELTASIZE;

   if (PlayingPointer==-1)
      {
      *ptr = NULL;
      *length = 0;
      return;
      }

   *ptr=&PlaybackBuffer[PlayingPointer];

   PlayingPointer = (PlayingPointer + *length) &
                      (PLAYBACKBUFFERSIZE - 1);
   }

//***************************************************************************
//
// SD_StartIncomingSound - Setup to receive an incoming sound in chunks
//
//***************************************************************************

void SD_StartIncomingSound ( void )
{
   if (SD_Started==FALSE)
      return;
   if ( ( Recording==TRUE ) || ( Playback==TRUE ) )
      {
      return;
      }

   Playback=TRUE;
   PlaybackBuffer = SafeMalloc (PLAYBACKBUFFERSIZE);
   Playing = FALSE;
   PlayingPointer = -1;
   PlaybackPointer = 0;

#if 0
   Playingvoice = FX_StartDemandFeedPlayback ( SD_UpdatePlaybackSound,
                  RECORDINGSAMPLERATE,
                  0, 255, 255, 255, 255, -1);
#endif
   if (Playingvoice==0)
      {
      SafeFree(PlaybackBuffer);
      Playback=FALSE;
      }
}

//***************************************************************************
//
// SD_StopIncomingSound - Stop receiving an incoming sound and playback
//
//***************************************************************************

void SD_StopIncomingSound ( void )
{
   if (SD_Started==FALSE)
      return;
   Playback=FALSE;
}


//***************************************************************************
//
// SD_UpdateIncomingSound - Update an incoming sound
//
//***************************************************************************

void SD_UpdateIncomingSound ( uint8_t * ptr, uint16_t length )
{
   int amount;

   if (SD_Started==FALSE)
      return;

   if ( Playback==FALSE )
      {
      return;
      }
   amount=length;
   if (PlaybackPointer+length > PLAYBACKBUFFERSIZE)
      amount=PLAYBACKBUFFERSIZE-PlaybackPointer;
   memcpy ( &PlaybackBuffer[PlaybackPointer],
            ptr, amount);
   PlaybackPointer = (PlaybackPointer + amount) &
                      (PLAYBACKBUFFERSIZE - 1);

   ptr+=amount;

   if (length!=amount)
      {
      amount=length-amount;
      memcpy ( &PlaybackBuffer[PlaybackPointer],
               ptr, amount);
      PlaybackPointer = (PlaybackPointer + amount) &
                         (PLAYBACKBUFFERSIZE - 1);
      }

   if (PlayingPointer==-1)
      {
      Playing=TRUE;
      PlayingPointer=0;
      }
   if (PlaybackPointer==PlayingPointer)
      {
      Playback=FALSE;
      }
}

//***************************************************************************
//
// SD_UpdateRecordingSound - Update recording a sound in chunks
//
//***************************************************************************
extern int whereami;
void SD_UpdateRecordingSound ( char * ptr, int length )
   {
   int amount;

   whereami = 69;
   if ( Recording==FALSE )
      {
      return;
      }
   whereami = 70;
   amount=length;
   if (RecordingPointer+length > RECORDINGBUFFERSIZE)
      amount=RECORDINGBUFFERSIZE-RecordingPointer;
   memcpy ( &RecordingBuffer[RecordingPointer],
            ptr, amount);
   whereami = 71;
   RecordingPointer = (RecordingPointer + amount) &
                      (RECORDINGBUFFERSIZE - 1);

   if (length!=amount)
      {
      ptr += amount;
      amount=length-amount;
      memcpy ( &RecordingBuffer[RecordingPointer],
               ptr, amount);
      RecordingPointer = (RecordingPointer + amount) &
                         (RECORDINGBUFFERSIZE - 1);
      }
   whereami = 72;
   if (Feeder == FALSE)
      {
      Feeder = TRUE;
      }

   whereami = 73;
   if (RecordingPointer==FeederPointer)
      {
      Recording=FALSE;
      }
   whereami = 74;
   }

//***************************************************************************
//
// SD_StartRecordingSound - Start recording a sound in chunks
//
//***************************************************************************

bool8_t SD_StartRecordingSound ( void )
{
   int status=FX_Ok;

   if (SD_Started==FALSE)
      return FALSE;
   if (remoteridicule == FALSE)
      return FALSE;
   if ( ( Recording==TRUE ) || ( Playback==TRUE ) || (Feeder==TRUE))
      {
      return FALSE;
      }
   Recording=TRUE;
   RecordingBuffer = SafeMalloc (RECORDINGBUFFERSIZE);
   Feeder = FALSE;
   FeederPointer = -1;
   RecordingPointer = 0;

#if 0
   status=FX_StartRecording( RECORDINGSAMPLERATE, SD_UpdateRecordingSound);
#endif

   if (status!=FX_Ok)
      {
      Recording=FALSE;
      SafeFree(RecordingBuffer);
      return FALSE;
      }

   return TRUE;
}

//***************************************************************************
//
// SD_StopRecordingSound - Stop recording a sound
//
//***************************************************************************

void SD_StopRecordingSound ( void )
{
   if (SD_Started==FALSE)
      return;
   if (Recording == TRUE)
      {
#if 0
      FX_StopRecord();
#endif
      Recording=FALSE;
      }
}

//***************************************************************************
//
// SD_SetRecordingActive - Set the recording active flag
//
//***************************************************************************

void SD_SetRecordingActive ( void )
{
   RecordingSemaphore=TRUE;
}

//***************************************************************************
//
// SD_ClearRecordingActive - Clear the recording active flag
//
//***************************************************************************

void SD_ClearRecordingActive ( void )
{
   RecordingSemaphore=FALSE;
}

//***************************************************************************
//
// SD_RecordingActive - Check if recording is active on some system
//
//***************************************************************************

bool8_t SD_RecordingActive ( void )
{
   return RecordingSemaphore;
}

//***************************************************************************
//
// SD_GetSoundData - Returns next piece of sound data, returns:
//
//                   nodata if no sound data is ready
//                   newsound if it is the start of a new sound
//                            data is also returned;
//                   endsound if the sound is finished
//                   data if data is ready
//
//***************************************************************************

recordstate SD_GetSoundData ( uint8_t * data, uint16_t length )
{
   recordstate status=rs_data;
   int amount;

   if (SD_Started==FALSE)
      return rs_nodata;

   if (Feeder==FALSE)
      return rs_nodata;

   if (FeederPointer==RecordingPointer)
      {
      if (Recording==FALSE)
         {
         SafeFree(RecordingBuffer);
         Feeder=FALSE;
         return rs_endsound;
         }
      else
         {
         return rs_nodata;
         }
      }

   if (FeederPointer==-1)
      {
      status=rs_newsound;
      FeederPointer=0;
      }

   amount=length;

   if (FeederPointer+length > RECORDINGBUFFERSIZE)
      amount=RECORDINGBUFFERSIZE-FeederPointer;
   memcpy ( data, &RecordingBuffer[FeederPointer], amount);

   FeederPointer = (FeederPointer + amount) &
                      (RECORDINGBUFFERSIZE - 1);

   data += amount;

   if (length!=amount)
      {
      amount=length-amount;
      memcpy ( data, &RecordingBuffer[FeederPointer], amount);
      FeederPointer = (FeederPointer + amount) &
                        (RECORDINGBUFFERSIZE - 1);
      }

   return status;
}

//***************************************************************************
//
// SD_SoundDataReady - Returns true if data is ready
//
//***************************************************************************

bool8_t SD_SoundDataReady ( void )
{
   if (SD_Started==FALSE)
      return FALSE;
   return Feeder;
}




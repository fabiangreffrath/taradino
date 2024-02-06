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
//***************************************************************************
//
//   RT_NET.H - Network stuff
//
//***************************************************************************

#ifndef _rt_net_public
#define _rt_net_public

#include <stdint.h>
#include "develop.h"
#include "rottnet.h"
#include "rt_actor.h"
#include "rt_battl.h"
#include "rt_playr.h"
#include "rt_main.h"


#define MAXCMDS  256

#define COM_DELTA   1
#define COM_REQUEST 2
#define COM_FIXUP   3
#define COM_TEXT    4
#define COM_PAUSE   5
#define COM_QUIT    6
#define COM_SYNC    7
#define COM_REMRID  8
#define COM_RESPAWN 10
#define COM_UNPAUSE 11
#define COM_SERVER  12
#define COM_START   13
#define COM_GAMEDESC 15
#define COM_GAMEPLAY 16
#define COM_GAMEMASTER 17
#define COM_GAMEACK 18
#define COM_ENDGAME  19
#define COM_SYNCTIME 20
#define COM_SOUNDANDDELTA  22
#define COM_EXIT    23
#define COM_GAMEEND 24
#define COM_DELTANULL 25

#define CHECKSYNCTIME  (VBLCOUNTER<<2)

#define NETSYNCSERVERTIME (VBLCOUNTER)
#define MODEMSYNCSERVERTIME (VBLCOUNTER/4)

#define DUMMYPACKETSIZE 20


#define COM_MAXTEXTSTRINGLENGTH 33

// Sound defines for Remote ridicule

#define COM_SOUND_START_TRANSMISSION  (0xff)
#define COM_SOUND_END_TRANSMISSION    (0xfe)
#define COM_SOUND_NORMAL_TRANSMISSION (0xfd)
#define COM_SOUND_BUFFERSIZE 256

// Demo Delta Structure
typedef struct DemoType {
  int   time;
  int16_t momx;
  int16_t momy;
  uint16_t  dangle;
  uint16_t  buttons;
} DemoType;

// Demo Header Structure
typedef struct DemoHeaderType {
  gametype demostate;
} DemoHeaderType;

// Movement Queue Structure
typedef struct MoveType {
  uint8_t  type;
  int   time;
  int16_t momx;
  int16_t momy;
  uint16_t  dangle;
  uint16_t  buttons;
  char  Sounddata[0];
} MoveType;

typedef struct NullMoveType {
  uint8_t  type;
  int   time;
} NullMoveType;

typedef struct {

  void * Commands[MAXCMDS];

} CommandType;

typedef struct {

  uint8_t CommandStates[MAXCMDS];

} CommandStatusType;

typedef MoveType COM_SoundAndDeltaType;

// uncomment for live remote ridicule
typedef struct {
  uint8_t  type;
  uint8_t  data[COM_SOUND_BUFFERSIZE];
//  char  data[COM_MAXTEXTSTRINGLENGTH];
} COM_SoundType;

typedef struct {
  uint8_t  type;
  int   synctime;
} COM_SyncType;

typedef struct {
  uint8_t  type;
  int   time;
  int   synctime;
  int   x;
  int   y;
  int   z;
  uint16_t  angle;
  uint16_t  randomindex;
} COM_CheckSyncType;

typedef struct {
  uint8_t  type;
  int   time;
  uint8_t  numpackets;
  uint8_t  data;
} COM_ServerHeaderType;

typedef struct {
  uint8_t  type;
  int   time;
  uint8_t   numpackets;
} COM_RequestType;

typedef struct {
  uint8_t  type;
  int   time;
  uint8_t  towho;
  char  string[COM_MAXTEXTSTRINGLENGTH];
} COM_TextType;

#define MSG_DIRECTED_TO_ALL  255
#define MSG_DIRECTED_TO_TEAM 254

typedef struct {
  uint8_t  type;
  int   time;
  uint8_t  player;
  uint8_t  num;
  uint8_t  towho;
} COM_RemoteRidiculeType;

typedef struct {
  uint8_t  type;
  int   time;
  uint8_t  numpackets;
  uint8_t  data;
} COM_FixupType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_QuitType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_ExitType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_GameEndType;

typedef struct {
  uint8_t  character;    // which character
  uint8_t  uniformcolor; // which color
  char  codename[MAXCODENAMELENGTH];  // codename
} COM_PlayerDescriptionType;

typedef struct {
  uint8_t  type;
  uint8_t  player;    // which player
  uint8_t  violence;
  uint8_t  Product;
  uint32_t Version;

  COM_PlayerDescriptionType playerdescription;
} COM_GamePlayerType;

typedef struct {
  uint8_t  type;
  uint8_t  level;
  uint16_t  mapcrc;
  uint8_t  violence;
  uint8_t  Product;
  uint8_t  mode;
  uint32_t Version;
  bool8_t teamplay;
  specials SpecialsTimes;
  battle_type options;
  char  battlefilename[20];
  int   randomseed;
  bool8_t ludicrousgibs;
  COM_PlayerDescriptionType players[MAXPLAYERS];
} COM_GameMasterType;

typedef struct {
  uint8_t  type;
  uint8_t  player; // which player
} COM_GameAckType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_EndGameType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_RespawnType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_PauseType;

typedef struct {
  uint8_t  type;
  int   time;
} COM_UnPauseType;

extern bool8_t  demorecord,
					 demoplayback;
extern uint8_t     *demoptr,
					 *lastdemoptr,
                *demobuffer;
extern bool8_t  demodone;
extern int      predemo_violence;

void     ControlPlayer (void);
void     ControlRemote (objtype * ob);
void     ControlPlayerObj (objtype * ob);
void InitializeGameCommands( void );
void ShutdownGameCommands( void );
void UpdateClientControls ( void );
void StartupClientControls ( void );
void ShutdownClientControls ( void );
void ProcessServer ( void );
void ServerLoop( void );
void SendPlayerDescription( void );
void SetGameDescription( void * pkt );
void SendGameDescription( void );
void SendGameAck( void );
void SendGameStart( void );
void SetupGamePlayer ( void );
void SetupGameMaster ( void );
void SetNormalHorizon (objtype * ob);
void SaveDemo (int demonumber);
void LoadDemo (int demonumber);
void RecordDemo ( void );
void SetupDemo ( void );
void FreeDemo ( void );
bool8_t DemoExists (int demonumber);

void AddEndGameCommand ( void );
void AddTextMessage ( char * message, int length, int towho );
void AddEndGameCommand ( void );
void AddPauseStateCommand ( int type );
void AddRespawnCommand ( void );
void RecordDemoCmd (void);
void ResetCurrentCommand ( void );
void AddRemoteRidiculeCommand ( int player, int towho, int num );
void ProcessRemoteRidicule ( void * pkt );
void SyncToServer( void );
void AddQuitCommand ( void );
void AddExitCommand ( void );
void AddGameEndCommand ( void );
bool8_t PlayerInGame ( int p );
bool8_t ConsoleIsServer ( void );

extern bool8_t IsServer;
extern bool8_t standalone;
extern bool8_t    playerdead;

extern bool8_t    modemgame;
extern bool8_t    networkgame;
extern int        numplayers;
extern int        server;

extern bool8_t    GamePaused;
extern bool8_t    battlegibs;

extern bool8_t    remoteridicule;

int GamePacketSize( void );

#endif

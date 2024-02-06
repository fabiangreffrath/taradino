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
//                  RT_MAIN.H
//
//***************************************************************************

#ifndef _rt_main_public
#define _rt_main_public

#include "develop.h"
#include "rt_def.h"
#include "rottnet.h"
#include "rt_battl.h"

#if (SHAREWARE==0)
   #define  STANDARDGAMELEVELS   "DARKWAR.RTL"
   #define  STANDARDBATTLELEVELS "DARKWAR.RTC"
   #define  SUPERROTTBATTLELEVELS "ROTTCD.RTC"
   #define  SITELICENSEBATTLELEVELS "ROTTSITE.RTC"
#else
   #define  STANDARDGAMELEVELS   "HUNTBGIN.RTL"
   #define  STANDARDBATTLELEVELS "HUNTBGIN.RTC"
#endif

enum
{vl_low,
 vl_medium,
 vl_high,
 vl_excessive
};

// Enum for each version of the game
typedef enum
   {
   ROTT_SHAREWARE,
   ROTT_REGISTERED,
   ROTT_SUPERCD,
   ROTT_SITELICENSE
   } version_type;

typedef struct
   {
   int GodModeTime;
   int DogModeTime;
   int ShroomsModeTime;
   int ElastoModeTime;
   int AsbestosVestTime;
   int BulletProofVestTime;
   int GasMaskTime;
   int MercuryModeTime;

   int GodModeRespawnTime;
   int DogModeRespawnTime;
   int ShroomsModeRespawnTime;
   int ElastoModeRespawnTime;
   int AsbestosVestRespawnTime;
   int BulletProofVestRespawnTime;
   int GasMaskRespawnTime;
   int MercuryModeRespawnTime;

   }specials;


typedef struct
   {
   uint32_t Version;
   // Variable for which version of the game can be played
   version_type Product;

   int     TimeCount;
   int     frame;
   int     secrettotal,treasuretotal,killtotal;
   int     secretcount,treasurecount,killcount;
   int     supertotal,healthtotal,missiletotal;
   int     supercount,healthcount,missilecount;
   int     democratictotal,planttotal;
   int     democraticcount,plantcount;
   int     dipballs;
   int     difficulty;
   int     violence;
	int     mapon;
	int     score;
	int     episode;
	int     battlemode;
	int     battleoption;
	int     randomseed;
   bool8_t teamplay;
	bool8_t DODEMOCRATICBONUS1;
	bool8_t DOGROUNDZEROBONUS;
	int     autorun;

	// Battle Options
   battle_type BattleOptions;

   bool8_t SpawnCollectItems;
	bool8_t SpawnEluder;
	bool8_t SpawnDeluder;
   bool8_t ShowScores;
	bool8_t PlayerHasGun[ MAXPLAYERS ];
   specials SpecialsTimes;
   } gametype;


extern  int      doublestep;
extern  bool8_t  tedlevel;
extern  int      tedlevelnum;
extern  int      tedx;
extern  int      tedy;
extern  bool8_t  fizzlein;
extern  int      pheight;
extern  int      NoSound;
extern  int      timelimit;
extern  bool8_t  timelimitenabled;
extern  bool8_t  noecho;
extern  bool8_t  demoexit;
extern  bool8_t  quiet;

extern gametype  gamestate;
extern bool8_t DebugOk;
extern  bool8_t newlevel;

void QuitGame( void );
void PlayCinematic (void);
void InitCharacter(void);
void ShutDown ( void );
void UpdateGameObjects ( void );

#if (WHEREAMI==1)
extern int programlocation;
#endif

extern  int polltime;
extern  int oldpolltime;
extern  volatile int oldtime;
void PauseLoop ( void );
void SaveScreen (bool8_t inhmenu);
void SetupWads( void );

extern bool8_t SCREENSHOTS;
extern bool8_t COMPUTELEVELSIZE;
extern bool8_t MONOPRESENT;
extern bool8_t MAPSTATS;
extern bool8_t TILESTATS;
extern bool8_t HUD;

extern char CWD[40];

#endif

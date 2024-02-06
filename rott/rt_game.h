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
#ifndef _rt_game_public
#define _rt_game_public

#include <stdint.h>

//***************************************************************************
//
// Public header for RT_GAME.C
//
//***************************************************************************

#include "rt_actor.h"
#include "lumpy.h"
#include "rt_cfg.h"
#include "rt_playr.h"

//***************************************************************************
//
// DEFINES
//
//***************************************************************************

#define  MaxHighName 57
#define  MaxScores   7


//***************************************************************************
//
// TYPEDEFS
//
//***************************************************************************

typedef struct
{
   char  message[30];
   uint8_t  episode;
   uint8_t  area;
   uint8_t  version;
   uint8_t  picture[16000];
   uint16_t  mapcrc;
   AlternateInformation info;
} gamestorage_t;

typedef  struct
{
	char  name[MaxHighName + 1];
	long  score;
	uint16_t  completed,episode;
} HighScore;


//***************************************************************************
//
// GLOBALS
//
//***************************************************************************

extern int PlayerSnds[5];

//extern int SHAKETICS;
extern uint16_t SHAKETICS;//bna++
extern int damagecount;

extern HighScore   Scores[MaxScores];
extern int SaveTime;

//***************************************************************************
//
// PROTOTYPES
//
//***************************************************************************

void SetupPlayScreen (void);
void SD_PreCache (void);
void GameMemToScreen( pic_t *source, int x, int y, int bufferofsonly );
void DrawPlayScreen (bool8_t bufferofsonly);

void DrawKills (bool8_t bufferofsonly);
void DrawPlayers ( void );
void DrawGameString (int x, int y, const char * str, bool8_t bufferofsonly);
void DrawNumber (int x, int y, int width, int which, bool8_t bufferofsonly);
void TakeDamage (int points, objtype *attacker);
void HealPlayer (int points, objtype * ob);
void DrawLives (bool8_t bufferofsonly);
void GiveExtraMan (void);
void DrawScore (bool8_t bufferofsonly);
void GivePoints (long points);
void DrawKeys (bool8_t bufferofsonly);
void GiveKey (int key);
void GiveWeapon (objtype * ob, int weapon);
void GiveMissileWeapon(objtype * ob, int which);
void GiveLives (int newlives);

void UpdateScore (uint32_t num);
void UpdateLives (int num);

void DrawTimeXY( int x, int y, int sec, bool8_t bufferofsonly );
void DrawTime (bool8_t bufferofsonly);

bool8_t SaveTheGame (int num, gamestorage_t * game);
bool8_t LoadTheGame (int num, gamestorage_t * game);
void GetSavedMessage (int num, char * message);
void GetSavedHeader (int num, gamestorage_t * game);

void DrawHighScores (void);
void CheckHighScore (long score, uint16_t other, bool8_t INMENU);
void LevelCompleted ( exit_t playstate );
void BattleLevelCompleted ( int localplayer );
void Died (void);
void ScreenShake (void);
void UpdateTriads (objtype * ob, int num);
void DrawTriads (bool8_t bufferofsonly);
void DrawStats (void);
void DrawBarHealth (bool8_t bufferonly);
void DrawBarAmmo (bool8_t bufferonly);
void GM_DrawBonus (int which);

void DrawEpisodeLevel ( int x, int y );
void DoBorderShifts (void);
void GM_UpdateBonus (int time, int powerup);
void DoLoadGameAction (void);
int GetLevel (int episode, int mapon);
void DrawPause (void);
void DrawPauseXY (int x, int y);

void DrawColoredMPPic (int xpos, int ypos, int width, int height, int heightmod, uint8_t *src, bool8_t bufferofsonly, int color);
void StatusDrawColoredPic (uint32_t x, uint32_t y, pic_t *nums, bool8_t bufferofsonly, int color);

void ClearTriads (playertype * pstate);
#endif

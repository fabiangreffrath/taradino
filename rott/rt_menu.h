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
#ifndef _rt_menu_public
#define _rt_menu_public

#include <stdint.h>

//******************************************************************************
//
// Public header for RT_MENU.C.
//
//******************************************************************************

#include "lumpy.h"
#include "rt_in.h"

#define MaxX            320
#define MaxY            200
#define MaxString       128

#define SaveGamePicX    18
#define SaveGamePicY    18

//******************************************************************************
//
// TYPEDEFS
//
//******************************************************************************

typedef enum
   {
   mn_tinyfont,
   mn_8x8font,
   mn_smallfont,
   mn_largefont
   } mn_fontsize;

typedef char CP_MenuNames[64];

typedef struct
   {
   int x;
   int y;
   int amount;
   int curpos;
   int indent;
   CP_MenuNames *names;
   mn_fontsize  fontsize;
   } CP_iteminfo;


typedef union
{
  void (*v);
  void (*vv)(void);
  void (*vi)(int); // CP_Quit()
  int (*iv)(void); // CP_SaveGame()
  int (*iii)(int, int); // CP_LoadGame()
} menuptr;

typedef struct
{
   int active;
   char texture[9];
   char letter;
   menuptr routine;
} CP_itemtype;

enum
   {
   CP_Inactive,
   CP_Active,
   CP_CursorLocation,
   CP_Active3
   };

//******************************************************************************
//
// GLOBALS
//
//******************************************************************************

extern char*colorname[];
extern bool8_t NewGame;

extern CP_itemtype MainMenu[];
extern bool8_t pickquick;

extern cfont_t *IFont;
extern font_t *CurrentFont;
extern font_t *tinyfont;

//
// Global window coords
//
extern int PrintX;
extern int PrintY;
extern int WindowX;
extern int WindowY;
extern int WindowH;
extern int WindowW;

extern int px;
extern int py;
extern int bufferheight;
extern int bufferwidth;
extern bool8_t loadedgame;
extern int FXMode;
extern int MusicMode;

extern font_t *newfont1;
extern font_t *smallfont;
extern font_t *bigfont;


extern bool8_t AutoDetailOn;
extern bool8_t DoubleClickOn;
extern bool8_t BobbinOn;
extern int     Menuflipspeed;
extern int     DetailLevel;

extern bool8_t          ingame;
extern bool8_t          inmenu;
extern int              scancode;

extern int quicksaveslot;

//****************************************************************************
//
// TYPEDEFS
//
//****************************************************************************
typedef enum
{
   newgame,
   battlemode,
   loadgame,
   savegame,
   control,
   orderinfo,
   viewscores,
   backtodemo,
   quit
} menuitems;



//****************************************************************************
//
// PROTOTYPES
//
//****************************************************************************

int  CP_ColorSelection(void);
void CP_BattleMode(void);
void DisplayInfo (int which);
bool8_t CP_DisplayMsg (char *s, int number);
void Message (char *string);
void DrawMenu (CP_iteminfo *item_i, CP_itemtype *items);
void DrawMainMenu(void);
void AllocateSavedScreenPtr (void);
void FreeSavedScreenPtr (void);
void CleanUpControlPanel (void);
void SetUpControlPanel (void);
void ControlPanel (uint8_t scancode);
menuitems CP_MainMenu( void );
int getASCII ( void );
void DoMainMenu (void);
bool8_t CP_CheckQuick (uint8_t scancode);
void AdjustMenuStruct (void);
void MenuFixup (void);
void GetEpisode (int level);
void MN_PlayMenuSnd (int which);
void CP_ViewScores (void);
void ReadAnyControl (ControlInfo *ci);
void WaitKeyUp (void);

void GetMenuInfo (void);
void WriteMenuInfo (void);
int GetNumActive (CP_iteminfo *item_i, CP_itemtype *items);
int MN_GetActive (CP_iteminfo *item_i, CP_itemtype *items, int check, int *nums);
void MN_MakeActive (CP_iteminfo *item_i, CP_itemtype *items, int which);
void MN_PlayMenuSnd (int which);

int CP_LoadGame (int quick, int dieload);

int CP_PlayerSelection (void);
void BattleGamePlayerSetup( void );
void BattleNoTeams( void );
void BattleTeams( void );
void CP_BattleMenu (void);
void CP_BattleModes (void);
void CP_ModemGameMessage ( int player );
void ShowBattleOptions( bool8_t inmenu, int PosX, int PosY );
void SetMenuHeader( char *header );
int  HandleMultiPageCustomMenu( char **names, int amount, int curpos, char *title, void ( *routine )( int w ), void ( *redrawfunc )( void ), bool8_t exitonselect );
int CP_LevelSelectionMenu ( void );
int CP_EnterCodeNameMenu ( void );
void QuickSaveGame (void);
void UndoQuickSaveGame (void);
void CP_CaptureTheTriadError( void );
void CP_TeamPlayErrorMessage( void );

#endif

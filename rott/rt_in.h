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
// Public header for RT_IN.C.
//
//***************************************************************************

#ifndef _rt_in_public
#define _rt_in_public

#include <stdbool.h>
#include <stdint.h>
#include "develop.h"
#include "rottnet.h"
#include "rt_def.h"

//***************************************************************************
//
// DEFINES
//
//***************************************************************************

#define MAXLETTERS   32


//***************************************************************************
//
// TYPEDEFS
//
//***************************************************************************

typedef  enum
{
	motion_Left  = -1,
	motion_Up    = -1,
   motion_None  = 0,
   motion_Right = 1,
   motion_Down  = 1
} Motion;


typedef  enum
{
   dir_North,
   dir_NorthEast,
   dir_East,
   dir_SouthEast,
   dir_South,
   dir_SouthWest,
   dir_West,
   dir_NorthWest,
   dir_None
} Direction;

typedef uint8_t ScanCode;

typedef  enum
{
	ctrl_Keyboard,
	ctrl_Keyboard1 = ctrl_Keyboard, ctrl_Keyboard2,
	ctrl_Joystick,
	ctrl_Joystick1 = ctrl_Joystick, ctrl_Joystick2,
	ctrl_Mouse
} ControlType;

typedef  struct
{
	bool     button0,
					button1,
					button2,
					button3;
	int         x,
					y;
	Motion      xaxis,
					yaxis;
	Direction   dir;
} CursorInfo;


typedef  CursorInfo  ControlInfo;


typedef  struct
{
   ScanCode button0,
            button1,
            upleft,
            up,
            upright,
            left,
            right,
            downleft,
            down,
            downright;
} KeyboardDef;


typedef struct
{
   uint16_t  joyMinX,joyMinY,
         threshMinX,threshMinY,
         threshMaxX,threshMaxY,
         joyMaxX,joyMaxY,
         joyMultXL,joyMultYL,
         joyMultXH,joyMultYH;
} JoystickDef;


typedef struct
{
   bool messageon;
   bool directed;
   bool inmenu;
   int     remoteridicule;
   int     towho;
   int     textnum;
   int     length;
} ModemMessage;


//***************************************************************************
//
// GLOBALS
//
//***************************************************************************

extern bool MousePresent;
extern bool JoysPresent[MaxJoys];
extern bool JoyPadPresent;
extern int     mouseadjustment;
extern int     threshold;

extern bool  Paused;
extern volatile int LastScan;
/* extern KeyboardDef KbdDefs;
extern JoystickDef JoyDefs[];
extern ControlType Controls[MAXPLAYERS]; */

extern char LastASCII;
extern volatile int LastScan;

extern uint8_t Joy_xb,
            Joy_yb,
            Joy_xs,
            Joy_ys;
extern uint16_t Joy_x,
            Joy_y;

extern int LastLetter;
extern char LetterQueue[MAXLETTERS];
extern ModemMessage MSG;

extern const char ScanChars[128];

//***************************************************************************
//
// PROTOTYPES
//
//***************************************************************************

void INL_GetMouseDelta(int *x,int *y);
uint16_t IN_GetMouseButtons (void);
void IN_IgnoreMouseButtons( void );
bool INL_StartMouse (void);
void INL_ShutMouse (void);
void IN_Startup(void);
void IN_Default (bool gotit, ControlType in);
void IN_Shutdown (void);
void IN_SetKeyHook(void (*hook)());
void IN_ClearKeysDown (void);
void IN_ReadControl (int player, ControlInfo *info);
void IN_SetControlType (int player, ControlType type);
ScanCode IN_WaitForKey (void);
char IN_WaitForASCII (void);
void IN_StartAck (void);
bool IN_CheckAck (void);
void IN_Ack (void);
bool IN_UserInput (long delay);
void IN_GetJoyAbs (uint16_t joy, uint16_t *xp, uint16_t *yp);
void INL_GetJoyDelta (uint16_t joy, int *dx, int *dy);
uint16_t INL_GetJoyButtons (uint16_t joy);
//uint16_t IN_GetJoyButtonsDB (uint16_t joy);
void INL_SetJoyScale (uint16_t joy);
void IN_SetupJoy (uint16_t joy, uint16_t minx, uint16_t maxx, uint16_t miny, uint16_t maxy);
bool INL_StartJoy (uint16_t joy);
void INL_ShutJoy (uint16_t joy);
uint8_t IN_JoyButtons (void);
void IN_UpdateKeyboard (void);
void IN_ClearKeyboardQueue (void);
int IN_InputUpdateKeyboard (void);
void IN_PumpEvents (void);
void QueueLetterInput (void);

#endif

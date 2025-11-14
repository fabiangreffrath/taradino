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
#ifndef _isr_public
#define _isr_public

#include "keyb.h"

//***************************************************************************
//
//    ISR.C - Interrupt Service Routines (Keyboard, timer)
//
//***************************************************************************

//***************************************************************************
//
//    ISR Constants
//
//***************************************************************************

#define VBLCOUNTER 35

#define KEYQMAX 256
#define MAXKEYBOARDSCAN 256

#define scroll_lock 0
#define num_lock 1
#define caps_lock 2

extern int KeyboardQueue[KEYQMAX];
extern int Keyhead;
extern int Keytail;

extern int Keyboard[MAXKEYBOARDSCAN]; // Keyboard status array
extern int Keystate[MAXKEYBOARDSCAN]; // Keyboard state array

int GetTicCount(void);

extern const int ASCIINames[]; // Ascii -> scan code conversion
extern const int ShiftNames[]; // Shifted Ascii->scancode conversion
extern boolean PausePressed;   // Game paused variable
extern boolean PanicPressed;   // Panic key variable

void I_Delay(int delay);
void ISR_SetTime(int settime);
void I_SendKeyboardData(int val);

#endif

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



/*
============================================================================

													 TIMER INTERRUPT

============================================================================
*/
#include <stdlib.h>
#include <stdio.h>

#include "rt_def.h"
#include "task_man.h"
#include "isr.h"
#include "_isr.h"
#include "rt_in.h"
#include "rt_util.h"
#include "profile.h"
#include "develop.h"
#include "rt_main.h"

#if (DEVELOPMENT == 1)

#include "rt_vid.h"

#endif
//MED
#include "memcheck.h"

// Global Variables

static volatile boolean ExtendedKeyFlag;

volatile int Keyboard[MAXKEYBOARDSCAN];
volatile int KeyboardQueue[KEYQMAX];
volatile int Keystate[MAXKEYBOARDSCAN];
volatile int Keyhead;
volatile int Keytail;

volatile boolean PausePressed = false;
volatile boolean PanicPressed = false;
int KeyboardStarted=false;

const int ASCIINames[] =          // Unshifted ASCII for scan codes
{
//       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
		  0  ,27 ,'1','2','3','4','5','6','7','8','9','0','-','=',8  ,9  ,        // 0
		  'q','w','e','r','t','y','u','i','o','p','[',']',13 ,0  ,'a','s',        // 1
		  'd','f','g','h','j','k','l',';',39 ,'`',0  ,92 ,'z','x','c','v',        // 2
		  'b','n','m',',','.','/',0  ,'*',0  ,' ',0  ,0  ,0  ,0  ,0  ,0  ,        // 3
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,'7','8','9','-','4','5','6','+','1',        // 4
		  '2','3','0',127,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,        // 5
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,        // 6
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0           // 7
};

const int ShiftNames[] =              // Shifted ASCII for scan codes
{
//       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
		  0  ,27 ,'!','@','#','$','%','^','&','*','(',')','_','+',8  ,9  ,        // 0
		  'Q','W','E','R','T','Y','U','I','O','P','{','}',13 ,0  ,'A','S',        // 1
		  'D','F','G','H','J','K','L',':',34 ,'~',0  ,'|','Z','X','C','V',        // 2
		  'B','N','M','<','>','?',0  ,'*',0  ,' ',0  ,0  ,0  ,0  ,0  ,0  ,        // 3
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,'7','8','9','-','4','5','6','+','1',        // 4
		  '2','3','0',127,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,        // 5
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,        // 6
		  0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,0           // 7
};

#include "SDL.h"

static int ticoffset;    /* offset for SDL_GetTicks() */
static int ticbase;      /* game-supplied base */

int GetTicCount (void)
{
	return ((SDL_GetTicks() - ticoffset) * VBLCOUNTER) / 1000 + ticbase;
}

void I_Sleep (int ms)
{
	SDL_Delay(ms);
}

/*
================
=
= ISR_SetTime
=
================
*/
void ISR_SetTime(int settime)
{
	ticoffset = SDL_GetTicks();
	ticbase = settime;
}

/* developer-only */

int GetFastTics (void)
{
	/* STUB_FUNCTION; */
	
	return 0;
}

void SetFastTics (int settime)
{
	/* STUB_FUNCTION; */
}

/*
================
=
= I_Delay
=
================
*/

void I_Delay ( int delay )
{
   int time;

   delay=(VBLCOUNTER*delay)/10;
   IN_ClearKeysDown();
   time=GetTicCount();
   while (!LastScan && !IN_GetMouseButtons() && GetTicCount()<time+delay)
      {
      	IN_UpdateKeyboard();
      }
}

/*
===============
=
= I_StartupTimer
=
===============
*/

void I_StartupTimer (void)
{
}

void I_ShutdownTimer (void)
{
}

/*
===============
=
= I_StartupKeyboard
=
===============
*/

void I_StartupKeyboard (void)
{
}


void I_ShutdownKeyboard (void)
{
}

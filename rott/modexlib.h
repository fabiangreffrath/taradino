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
//    MODEXLIB.C - various utils palette funcs and modex stuff
//
//***************************************************************************

#ifndef _modexlib_public
#define _modexlib_public

#include <stdbool.h>
#include <stdint.h>
#include "WinRott.h"
#include "rt_def.h"

//***************************************************************************
//
//    Video (ModeX) Constants
//
//***************************************************************************

extern  bool StretchScreen;

//extern  int      ylookup[MAXSCREENHEIGHT];      // Table of row offsets
extern  int      ylookup[600];      // just set to max res
extern  int      linewidth;
extern  uint8_t    *page1start;
extern  uint8_t    *page2start;
extern  uint8_t    *page3start;
extern  int      screensize;
extern  uint8_t    *bufferofs;
extern  uint8_t    *displayofs;
extern  bool  graphicsmode;


void  GraphicsMode ( void );
void  SetTextMode ( void );
void  VL_SetVGAPlaneMode ( void );
void  VL_ClearBuffer (uint8_t *buf, uint8_t color);
void  VL_ClearVideo (uint8_t color);
void  VL_DePlaneVGA (void);
void  VL_CopyDisplayToHidden ( void );
void  VL_CopyBufferToAll ( uint8_t *buffer );
void  VL_CopyPlanarPage ( uint8_t * src, uint8_t * dest );
void  VL_CopyPlanarPageToMemory ( uint8_t * src, uint8_t * dest );
void  XFlipPage ( void );
void  WaitVBL( void );
void  TurnOffTextCursor ( void );
void  ToggleFullScreen (void);
void  SetShowCursor (int);

#define VGAMAPMASK(a)
#define VGAREADMAP(a)
#define VGAWRITEMAP(a)

#endif

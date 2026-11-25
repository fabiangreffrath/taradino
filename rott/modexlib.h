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

#include "WinRott.h"
#include "rt_def.h"

//***************************************************************************
//
//    Video (ModeX) Constants
//
//***************************************************************************

extern boolean StretchScreen;

// Size of the original screen, in which all UI coordinates are expressed.
#define BASEWIDTH 320
#define BASEHEIGHT 200

// Factor between UI coordinates and real pixels (1 while stretching).
extern int uiscale;

// Integer factor between the base resolution and the real one.
static inline int GetScreenScale(void)
{
	return iGLOBAL_SCREENWIDTH / BASEWIDTH;
}

// extern  int      ylookup[MAXSCREENHEIGHT];      // Table of row offsets
extern int ylookup[600]; // just set to max res
extern int linewidth;
extern byte *SCREEN_BUFFER;
extern int screensize;
extern byte *bufferofs;
extern byte *displayofs;
extern boolean graphicsmode;

void GraphicsMode(void);
void SetTextMode(void);
void VL_SetVGAPlaneMode(void);
void VL_ClearBuffer(byte *buf, byte color);
void VL_ClearVideo(byte color);
void VL_CopyPlanarPage(byte *src, byte *dest);
void VL_CopyPlanarPageToMemory(byte *src, byte *dest);
void XFlipPage(void);
void WaitVBL(void);
void TurnOffTextCursor(void);
void ToggleFullScreen(void);
void SetShowCursor(int);

#define VGAMAPMASK(a)
#define VGAREADMAP(a)
#define VGAWRITEMAP(a)

// Fills the block that the base position (x, y) covers on the real screen.
static inline void PutScaledPixel(byte *base, int x, int y, byte color)
{
	byte *dest = base + ylookup[y * uiscale] + x * uiscale;

	for (int i = 0; i < uiscale; i++, dest += linewidth)
	{
		memset(dest, color, uiscale);
	}
}

#endif

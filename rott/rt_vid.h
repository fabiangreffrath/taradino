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
#ifndef _rt_vid_public
#define _rt_vid_public

#include <stdbool.h>
#include <stdint.h>

//***************************************************************************
//
// Public header for RT_VID.C
//
//***************************************************************************

#include "lumpy.h"

//***************************************************************************
//
// DEFINES
//
//***************************************************************************

#define PORTTILESWIDE      20       // all drawing takes place inside a
#define PORTTILESHIGH      13       // non displayed port of this size

#define UPDATEWIDE         PORTTILESWIDE
#define UPDATEHIGH         PORTTILESHIGH
#define UPDATESIZE         (UPDATEWIDE*UPDATEHIGH)


//***************************************************************************
//
// GLOBALS
//
//***************************************************************************

extern uint8_t     *updateptr;
extern unsigned mapwidthtable[64];
extern unsigned uwidthtable[UPDATEHIGH];
extern unsigned blockstarts[UPDATEWIDE*UPDATEHIGH];
extern uint8_t     update[UPDATESIZE];
extern bool  screenfaded;


//***************************************************************************
//
// PROTOTYPES
//
//***************************************************************************
void VL_MemStrechedToScreen (uint8_t *source, int width, int height, int x, int y);
void VL_MemToScreen (uint8_t *source, int width, int height, int x, int y);
void DrawTiledRegion( int x, int y, int width, int height, int offx, int offy, pic_t *tile );
void VWB_DrawPic (int x, int y, pic_t *pic);
void VL_Bar (int x, int y, int width, int height, int color);
void VWB_Bar (int x, int y, int width, int height, int color);
void VWB_Hlin (int x1, int x2, int y, int color);
void VWB_Vlin (int y1, int y2, int x, int color);
void VWB_THlin (int x1, int x2, int y, bool up);
void VWB_TVlin (int y1, int y2, int x, bool up);
int VW_MarkUpdateBlock (int x1, int y1, int x2, int y2);
void VW_UpdateScreen (void);

void VL_FadeOut (int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn (int start, int end, uint8_t *palette, int steps);
void VL_DecompressLBM (lbm_t *lbminfo, bool flip);
void VL_FadeToColor (int time, int red, int green, int blue);
void VWB_TBar (int x, int y, int width, int height);

void SwitchPalette (uint8_t * newpal, int steps);
void SetBorderColor (int color);

void VL_DrawPostPic (int lumpnum);
void VL_DrawLine (int x1, int y1, int x2, int y2, uint8_t color);

#define MenuFadeOut()	VL_FadeOut (0, 255, 0, 0, 0, 10)
#define MenuFadeIn()	   VL_FadeIn (0, 255, origpal, 10)

void DrawXYPic (int x, int y, int shapenum);

#endif

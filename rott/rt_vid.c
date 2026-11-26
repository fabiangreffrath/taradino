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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rt_def.h"
#include "rt_vid.h"
#include "rt_menu.h"
#include "rt_util.h"
#include "modexlib.h"
#include "profile.h"
#include "rt_str.h"
#include "rt_draw.h"
#include "rt_in.h"
#include "rt_main.h"
#include "z_zone.h"
#include "lumpy.h"
#include "isr.h"
#include "rt_view.h"
#include "cin_efct.h"
#include "w_wad.h"

//******************************************************************************
//
// DEFINES
//
//******************************************************************************

#define VW_Hlin(x, z, y, c) VL_Hlin(x, y, (z) - (x) + 1, c)
#define VW_Vlin(y, z, x, c) VL_Vlin(x, y, (z) - (y) + 1, c)

#define VW_THlin(x, z, y, up) VL_THlin(x, y, (z) - (x) + 1, up)
#define VW_TVlin(y, z, x, up) VL_TVlin(x, y, (z) - (y) + 1, up)

//******************************************************************************
//
// GLOBALS
//
//******************************************************************************

unsigned int mapwidthtable[64];
byte palette1[256][3], palette2[256][3];
boolean screenfaded;

//******************************************************************************
//
// VL_MemToScreen ()
//
//******************************************************************************

void VL_MemToScreen(byte *source, int width, int height, int x, int y)
{
	byte *ptr = source;

	for (int plane = 0; plane < 4; plane++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				PutScaledPixel(bufferofs, x + i * 4 + plane, y + j, *ptr++);
			}
		}
	}
}
void VL_MemToScreenClipped(byte *source, int width, int height, int x, int y);
void VL_MemToScreenClipped(byte *source, int width, int height, int x, int y)
{
	byte *ptr = source;

	for (int plane = 0; plane < 4; plane++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++, ptr++)
			{
				if (*ptr != 255)
				{
					PutScaledPixel(bufferofs, x + i * 4 + plane, y + j, *ptr);
				}
			}
		}
	}
}


//*************************************************************************
//
// DrawTiledRegion () - Fills the specified region with a tiled image, scaled
// like the rest of the interface
//
//*************************************************************************
void DrawTiledRegion(int x, int y, int width, int height, int offx, int offy,
					 pic_t *tile)
{
	const int scale = GetScreenScale();
	const byte *const source = &tile->data;
	const int planesize = tile->width * tile->height;

	for (int j = 0; j < height; j++)
	{
		const int row = ((offy + j) / scale) % tile->height;
		byte *const dest = bufferofs + x + ylookup[y + j];

		for (int i = 0; i < width; i++)
		{
			const int column = i / scale;
			const int plane = column % 4;
			const int tilex = (offx + column / 4) % tile->width;

			dest[i] = source[plane * planesize + row * tile->width + tilex];
		}
	}
}

//******************************************************************************
//
// VWB_DrawPic () - Draws a linear pic and marks the update block
//
//******************************************************************************

void VWB_DrawPic(int x, int y, pic_t *pic)
{
	VL_MemToScreen((byte *)&pic->data, pic->width, pic->height, x, y);
}

//******************************************************************************
//
// VL_Bar () - Draws a bar
//
//******************************************************************************

void VL_Bar(int x, int y, int width, int height, int color)
{
	byte *dest = (byte *)(bufferofs + ylookup[y] + x);

	while (height--)
	{
		memset(dest, color, width);

		dest += linewidth;
	}
}

//******************************************************************************
//
// VWB_Bar () - Draws a block and marks the update block
//
//******************************************************************************

void VWB_Bar(int x, int y, int width, int height, int color)
{
	VL_Bar(x * uiscale, y * uiscale, width * uiscale, height * uiscale, color);
}

//******************************************************************************
//
// VL_TBar () - Draws a bar
//
//******************************************************************************

void VL_TBar(int x, int y, int width, int height)
{
	int w = width;

	while (height--)
	{
		byte *dest = (byte *)(bufferofs + ylookup[y] + x);

		width = w;

		while (width--)
		{
			byte pixel = *dest;

			pixel = *(colormap + (27 << 8) + pixel);

			*dest = pixel;

			dest++;
		}

		y++;
	}
}

//******************************************************************************
//
// VWB_TBar () - Draws a block and marks the update block
//
//******************************************************************************

void VWB_TBar(int x, int y, int width, int height)
{
	VL_TBar(x * uiscale, y * uiscale, width * uiscale, height * uiscale);
}

//******************************************************************************
//
// VL_Hlin () - Draws a horizontal line
//
//******************************************************************************

void VL_Hlin(unsigned int x, unsigned int y, unsigned int width,
			 unsigned int color)
{
	byte *dest = (byte *)(bufferofs + ylookup[y] + x);

	memset(dest, color, width);
}

//******************************************************************************
//
// VL_Vlin () - Draws a vertical line
//
//******************************************************************************

void VL_Vlin(int x, int y, int height, int color)
{
	byte *dest = (byte *)(bufferofs + ylookup[y] + x);

	while (height--)
	{
		*dest = color;

		dest += linewidth;
	}
}

//******************************************************************************
//
// VWB_Hlin () - Draws a horizontal line and marks the update block
//
//******************************************************************************

void VWB_Hlin(int x1, int x2, int y, int color)
{
	VL_Bar(x1 * uiscale, y * uiscale, (x2 - x1 + 1) * uiscale, uiscale, color);
}

//******************************************************************************
//
// VWB_Vlin () - Draws a vertical line and marks the update block
//
//******************************************************************************

void VWB_Vlin(int y1, int y2, int x, int color)
{
	VL_Bar(x * uiscale, y1 * uiscale, uiscale, (y2 - y1 + 1) * uiscale, color);
}

//******************************************************************************
//
// VL_THlin ()
//
//******************************************************************************

void VL_THlin(unsigned int x, unsigned int y, unsigned int width, boolean up)
{
	byte *dest = (byte *)(bufferofs + ylookup[y] + x);

	while (width--)
	{
		byte pixel = *dest;

		if (up)
		{
			pixel = *(colormap + (13 << 8) + pixel);
		}
		else
		{
			pixel = *(colormap + (27 << 8) + pixel);
		}

		*dest = pixel;

		dest++;
	}
}

//******************************************************************************
//
// VL_TVlin ()
//
//******************************************************************************

void VL_TVlin(unsigned int x, unsigned int y, unsigned int height, boolean up)
{
	byte *dest = (byte *)(bufferofs + ylookup[y] + x);

	while (height--)
	{
		byte pixel = *dest;

		if (up)
		{
			pixel = *(colormap + (13 << 8) + pixel);
		}
		else
		{
			pixel = *(colormap + (27 << 8) + pixel);
		}

		*dest = pixel;

		dest += linewidth;
	}
}

//******************************************************************************
//
// VWB_THlin () - Draws a horizontal line and marks the update block
//
//******************************************************************************

void VWB_THlin(int x1, int x2, int y, boolean up)
{
	for (int i = 0; i < uiscale; i++)
		VL_THlin(x1 * uiscale, y * uiscale + i, (x2 - x1 + 1) * uiscale, up);
}

//******************************************************************************
//
// VWB_TVlin () - Draws a vertical line and marks the update block
//
//******************************************************************************

void VWB_TVlin(int y1, int y2, int x, boolean up)
{
	for (int i = 0; i < uiscale; i++)
		VL_TVlin(x * uiscale + i, y1 * uiscale, (y2 - y1 + 1) * uiscale, up);
}

/*
================================================================================

			Double buffer management routines

================================================================================
*/

//******************************************************************************
//
// VW_UpdateScreen ()
//
//******************************************************************************

void VW_UpdateScreen(void)
{
	VH_UpdateScreen();
}

//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start, int end, int red, int green, int blue, int steps)
{
	int i, j, orig, delta;
	byte *origptr, *newptr;

	if (screenfaded)
		return;

	VH_UpdateScreen();
	WaitVBL();
	VL_GetPalette(&palette1[0][0]);
	memcpy(palette2, palette1, 768);

	//
	// fade through intermediate frames
	//
	for (i = 0; i < steps; i++)
	{
		origptr = &palette1[start][0];
		newptr = &palette2[start][0];

		for (j = start; j <= end; j++)
		{
			orig = *origptr++;
			delta = red - orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = green - orig;
			*newptr++ = orig + delta * i / steps;
			orig = *origptr++;
			delta = blue - orig;
			*newptr++ = orig + delta * i / steps;
		}

		VH_UpdateScreen();
		WaitVBL();
		VL_SetPalette(&palette2[0][0]);
	}

	//
	// final color
	//
	VL_FillPalette(red, green, blue);
	VH_UpdateScreen();

	screenfaded = true;
}

/*
=================
=
= VL_FadeToColor
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeToColor(int time, int red, int green, int blue)
{
	int i, j, orig, delta;
	byte *origptr, *newptr;
	int dmax, dmin;

	if (screenfaded)
		return;

	VH_UpdateScreen();
	WaitVBL();
	VL_GetPalette(&palette1[0][0]);
	memcpy(palette2, palette1, 768);

	dmax = (maxshade << 16) / time;
	dmin = (minshade << 16) / time;
	//
	// fade through intermediate frames
	//
	for (i = 0; i < time; i += tics)
	{
		origptr = &palette1[0][0];
		newptr = &palette2[0][0];

		for (j = 0; j <= 255; j++)
		{
			orig = *origptr++;
			delta = ((red >> 2) - orig) << 16;
			*newptr++ = orig + FixedMul(delta / time, i);
			orig = *origptr++;
			delta = ((green >> 2) - orig) << 16;
			*newptr++ = orig + FixedMul(delta / time, i);
			orig = *origptr++;
			delta = ((blue >> 2) - orig) << 16;
			*newptr++ = orig + FixedMul(delta / time, i);
		}

		maxshade = (dmax * (time - i)) >> 16;
		minshade = (dmin * (time - i)) >> 16;
		VH_UpdateScreen();
		WaitVBL();
		VL_SetPalette(&palette2[0][0]);
		ThreeDRefresh();
		CalcTics();
	}

	//
	// final color
	//
	VL_FillPalette(red >> 2, green >> 2, blue >> 2);
	VH_UpdateScreen();

	screenfaded = true;
}

/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn(int start, int end, byte *palette, int steps)
{
	int i, j, delta;

	VH_UpdateScreen();
	WaitVBL();
	VL_GetPalette(&palette1[0][0]);

	memcpy(&palette2[0][0], &palette1[0][0], sizeof(palette1));

	start *= 3;
	end = end * 3 + 2;

	//
	// fade through intermediate frames
	//
	for (i = 0; i < steps; i++)
	{
		for (j = start; j <= end; j++)
		{
			delta = palette[j] - (&palette1[0][0])[j];
			(&palette2[0][0])[j] = (&palette1[0][0])[j] + delta * i / steps;
		}

		VH_UpdateScreen();
		WaitVBL();
		VL_SetPalette(&palette2[0][0]);
	}

	//
	// final color
	//
	VL_SetPalette(palette);
	VH_UpdateScreen();

	screenfaded = false;
}

//******************************************************************************
//
// SwitchPalette
//
//******************************************************************************

void SwitchPalette(byte *newpal, int steps)
{
	byte *temp;

	VL_FadeOut(0, 255, 0, 0, 0, steps >> 1);

	temp = bufferofs;
	bufferofs = displayofs;
	VL_Bar(0, 0, 320, 200, 0);
	bufferofs = temp;

	VL_FadeIn(0, 255, newpal, steps >> 1);
}

//==========================================================================

//****************************************************************************
//
// VL_DecompressLBM ()
//
// LIMITATIONS - Only works with 320x200!!!
//
//****************************************************************************

void VL_DecompressLBM(lbm_t *lbminfo, boolean flip)
{
	int count;
	byte b, rept;
	byte *source = (byte *)&lbminfo->data;
	byte *buf;
	int ht = lbminfo->height;
	byte pal[768];

	EnableScreenStretch();

	memcpy(&pal[0], lbminfo->palette, 768);

	VL_NormalizePalette(&pal[0]);

	buf = (byte *)bufferofs;

	while (ht--)
	{
		count = 0;

		do
		{
			rept = *source++;

			if (rept > 0x80)
			{
				rept = (rept ^ 0xff) + 2;
				b = *source++;
				memset(buf, b, rept);
				buf += rept;
			}
			else if (rept < 0x80)
			{
				rept++;
				memcpy(buf, source, rept);
				buf += rept;
				source += rept;
			}
			else
				rept = 0; // rept of 0x80 is NOP

			count += rept;

		} while (count < lbminfo->width);
		buf += iGLOBAL_SCREENWIDTH - BASEWIDTH;
	}

	if (flip == true)
		VW_UpdateScreen();

	VL_FadeIn(0, 255, &pal[0], 15);
}

//****************************************************************************
//
// SetBorderColor
//
//****************************************************************************

void SetBorderColor(int color)
{

	byte *cnt, *Ycnt, *b;
	const int border = 5 * GetScreenScale();

	b = (byte *)bufferofs;

	// color 56 could be used

	// paint top red line
	for (cnt = b; cnt < b + viewwidth; cnt++)
	{
		for (Ycnt = cnt; Ycnt < cnt + (border * iGLOBAL_SCREENWIDTH);
			 Ycnt += iGLOBAL_SCREENWIDTH)
		{
			*Ycnt = color;
		}
	}
	// paint left red line
	for (cnt = b; cnt < b + border; cnt++)
	{
		for (Ycnt = cnt; Ycnt < cnt + (viewheight * iGLOBAL_SCREENWIDTH);
			 Ycnt += iGLOBAL_SCREENWIDTH)
		{
			*Ycnt = color;
		}
	}
	// paint right red line
	for (cnt = b + (viewwidth - border); cnt < b + viewwidth; cnt++)
	{
		for (Ycnt = cnt; Ycnt < cnt + (viewheight * iGLOBAL_SCREENWIDTH);
			 Ycnt += iGLOBAL_SCREENWIDTH)
		{
			*Ycnt = color;
		}
	}
	// paint lower red line
	for (cnt = b + ((viewheight - border) * iGLOBAL_SCREENWIDTH);
		 cnt < b + ((viewheight - border) * iGLOBAL_SCREENWIDTH) + viewwidth;
		 cnt++)
	{
		for (Ycnt = cnt; Ycnt < b + (viewheight * iGLOBAL_SCREENWIDTH);
			 Ycnt += iGLOBAL_SCREENWIDTH)
		{
			*Ycnt = color;
		}
	}
}

//****************************************************************************
//
// VL_DrawPostPic
//
//****************************************************************************

void VL_DrawPostPic(int lumpnum)
{
	DrawPostPic(lumpnum);
}

//****************************************************************************
//
// VL_DrawLine
//
//****************************************************************************

void VL_DrawLine(int x1, int y1, int x2, int y2, byte color)
{
	int dx;
	int dy;
	int xinc;
	int yinc;
	int count;

	dx = (x2 - x1);
	dy = (y2 - y1);
	if (abs(dy) >= abs(dx))
	{
		count = abs(dy);
		if (count == 0)
			return;
		yinc = (dy << 16) / count;
		if (dy == 0)
		{
			return;
		}
		else
		{
			xinc = (dx << 16) / count;
		}
	}
	else
	{
		count = abs(dx);
		if (count == 0)
			return;
		xinc = (dx << 16) / count;
		if (dx == 0)
		{
			return;
		}
		else
		{
			yinc = (dy << 16) / count;
		}
	}
	x1 <<= 16;
	y1 <<= 16;
	while (count > 0)
	{
		*((byte *)bufferofs + (x1 >> 16) + (ylookup[y1 >> 16])) = color;
		x1 += xinc;
		y1 += yinc;
		count--;
	}
}

//******************************************************************************
//
// DrawXYPic
//
//******************************************************************************

void DrawXYPic(int x, int y, int shapenum)
{
	const pic_t *const p =
		(pic_t *)W_CacheLumpNum(shapenum, PU_CACHE, Cvt_pic_t, 1);
	const byte *src = (byte *)&p->data;

	if ((x < 0) || ((x + (p->width << 2)) >= BASEWIDTH))
		Error("DrawXYPic: x is out of range\n");
	if ((y < 0) || ((y + p->height) >= BASEHEIGHT))
		Error("DrawXYPic: y is out of range\n");

	for (int plane = 0; plane < 4; plane++)
	{
		for (int yy = 0; yy < p->height; yy++)
		{
			for (int xx = 0; xx < p->width; xx++)
			{
				PutScaledPixel(bufferofs, x + plane + xx * 4, y + yy, *src++);
			}
		}
	}
}

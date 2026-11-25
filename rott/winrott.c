/* Routines from winrott needed for the highres support for the SDL port */
#include <stdlib.h>
#include <string.h>
#include "WinRott.h"
#include "modexlib.h"

// typedef unsigned char byte;

int iGLOBAL_SCREENWIDTH = 640;	// bna
int iGLOBAL_SCREENHEIGHT = 400; // bna

int iGLOBAL_HEALTH_X;
int iGLOBAL_HEALTH_Y;
int iGLOBAL_AMMO_X;
int iGLOBAL_AMMO_Y;

int iGLOBAL_FOCALWIDTH;
double dGLOBAL_FPFOCALWIDTH;

boolean iG_aimCross = 0;

extern int viewheight;
extern int viewwidth;

//----------------------------------------------------------------------
#define FINEANGLES 2048
extern int YZANGLELIMIT;
void SetRottScreenRes(int Width, int Height)
{

	iGLOBAL_SCREENWIDTH = Width;
	iGLOBAL_SCREENHEIGHT = Height;

	iGLOBAL_HEALTH_X = 20;
	iGLOBAL_HEALTH_Y = 185;
	iGLOBAL_AMMO_X = 300;
	iGLOBAL_AMMO_Y = 184;

	const int scale = GetScreenScale();

	iGLOBAL_FOCALWIDTH = 160 + 20 * (scale - 1);
	dGLOBAL_FPFOCALWIDTH = iGLOBAL_FOCALWIDTH;

	YZANGLELIMIT = (30 * scale * FINEANGLES / 360);
}

extern int viewsize;

// Shifts the view by a few pixels, the signs of dirx and diry give the way.
void MoveScreen(int dirx, int diry)
{
	const int scale = GetScreenScale();
	const int shift = 3 * scale;
	const int rows = viewheight - shift;
	const int width = viewwidth - shift;
	const int dstx = dirx > 0 ? shift : 0;
	const int srcx = dirx > 0 ? 0 : shift;
	const int dsty = diry > 0 ? shift : 0;
	const int srcy = diry > 0 ? 0 : shift;
	byte *base = (byte *)bufferofs;

	base += (((iGLOBAL_SCREENHEIGHT - viewheight) / 2) * iGLOBAL_SCREENWIDTH) +
			((iGLOBAL_SCREENWIDTH - viewwidth) / 2);
	if (viewsize == 8)
		base += 8 * scale * iGLOBAL_SCREENWIDTH;

	// Moving down copies from the bottom up so that no source is overwritten.
	for (int i = 0; i < rows; i++)
	{
		const int row = diry > 0 ? rows - 1 - i : i;

		memcpy(base + (row + dsty) * iGLOBAL_SCREENWIDTH + dstx,
			   base + (row + srcy) * iGLOBAL_SCREENWIDTH + srcx, width);
	}
}

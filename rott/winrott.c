/* Routines from winrott needed for the highres support for the SDL port */
#include <stdlib.h>
#include <string.h>
#include "WinRott.h"
#include "modexlib.h"

// typedef unsigned char byte;

int iGLOBAL_SCREENWIDTH = 640;	// bna
int iGLOBAL_SCREENHEIGHT = 400; // bna
int iGLOBAL_SCREENBWIDE;
int iG_SCREENWIDTH; // default screen width in bytes

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

	iGLOBAL_SCREENBWIDE = iGLOBAL_SCREENWIDTH * (96 / 320);
	iG_SCREENWIDTH = iGLOBAL_SCREENWIDTH * (96 / 320);
	; // default screen width in bytes

	iGLOBAL_HEALTH_X = 20;
	iGLOBAL_HEALTH_Y = 185;
	iGLOBAL_AMMO_X = 300;
	iGLOBAL_AMMO_Y = 184;

	const int scale = GetScreenScale();

	iGLOBAL_FOCALWIDTH = 160 + 20 * (scale - 1);
	dGLOBAL_FPFOCALWIDTH = iGLOBAL_FOCALWIDTH;

	YZANGLELIMIT = (30 * scale * FINEANGLES / 360);
}

//----------------------------------------------------------------------
// luckey for me that I am not programmin a 386 or the next
// 4 function would never have worked. bna++
extern int viewsize;
void MoveScreenUpLeft()
{
	int startX, startY, startoffset;
	byte *Ycnt, *b;
	//   SetTextMode (  );
	b = (byte *)bufferofs;
	b += (((iGLOBAL_SCREENHEIGHT - viewheight) / 2) * iGLOBAL_SCREENWIDTH) +
		 ((iGLOBAL_SCREENWIDTH - viewwidth) / 2);
	if (viewsize == 8)
	{
		b += 8 * GetScreenScale() * iGLOBAL_SCREENWIDTH;
	}
	startX = 3 * GetScreenScale();
	startY = 3 * GetScreenScale();
	startoffset = (startY * iGLOBAL_SCREENWIDTH) + startX;

	for (Ycnt = b; Ycnt < b + ((viewheight - startY) * iGLOBAL_SCREENWIDTH);
		 Ycnt += iGLOBAL_SCREENWIDTH)
	{
		memcpy(Ycnt, Ycnt + startoffset, viewwidth - startX);
	}
}
//----------------------------------------------------------------------
void MoveScreenDownLeft()
{
	int startX, startY, startoffset;
	byte *Ycnt, *b;
	//   SetTextMode (  );
	b = (byte *)bufferofs;
	b += (((iGLOBAL_SCREENHEIGHT - viewheight) / 2) * iGLOBAL_SCREENWIDTH) +
		 ((iGLOBAL_SCREENWIDTH - viewwidth) / 2);
	if (viewsize == 8)
	{
		b += 8 * GetScreenScale() * iGLOBAL_SCREENWIDTH;
	}
	startX = 3 * GetScreenScale();
	startY = 3 * GetScreenScale();
	startoffset = (startY * iGLOBAL_SCREENWIDTH); //+startX;

	// Ycnt starts in botton of screen and copys lines upwards
	for (Ycnt = b + ((viewheight - startY - 1) * iGLOBAL_SCREENWIDTH); Ycnt > b;
		 Ycnt -= iGLOBAL_SCREENWIDTH)
	{
		memcpy(Ycnt + startoffset, Ycnt + startX, viewwidth - startX);
	}
}
//----------------------------------------------------------------------
void MoveScreenUpRight()
{
	int startX, startY, startoffset;
	byte *Ycnt, *b;
	//   SetTextMode (  );
	b = (byte *)bufferofs;

	b += (((iGLOBAL_SCREENHEIGHT - viewheight) / 2) * iGLOBAL_SCREENWIDTH) +
		 ((iGLOBAL_SCREENWIDTH - viewwidth) / 2);
	if (viewsize == 8)
	{
		b += 8 * GetScreenScale() * iGLOBAL_SCREENWIDTH;
	}
	startX = 3 * GetScreenScale();
	startY = 3 * GetScreenScale();
	startoffset = (startY * iGLOBAL_SCREENWIDTH); //+startX;

	for (Ycnt = b; Ycnt < b + ((viewheight - startY) * iGLOBAL_SCREENWIDTH);
		 Ycnt += iGLOBAL_SCREENWIDTH)
	{
		memcpy(Ycnt + startX, Ycnt + startoffset, viewwidth - startX);
	}
}
//----------------------------------------------------------------------
void MoveScreenDownRight()
{
	int startX, startY, startoffset;
	byte *Ycnt, *b;
	//   SetTextMode (  );
	b = (byte *)bufferofs;

	b += (((iGLOBAL_SCREENHEIGHT - viewheight) / 2) * iGLOBAL_SCREENWIDTH) +
		 ((iGLOBAL_SCREENWIDTH - viewwidth) / 2);
	if (viewsize == 8)
	{
		b += 8 * GetScreenScale() * iGLOBAL_SCREENWIDTH;
	}
	startX = 3 * GetScreenScale();
	startY = 3 * GetScreenScale();
	startoffset = (startY * iGLOBAL_SCREENWIDTH) + startX;

	// Ycnt starts in botton of screen and copys lines upwards
	for (Ycnt = b + ((viewheight - startY - 1) * iGLOBAL_SCREENWIDTH); Ycnt > b;
		 Ycnt -= iGLOBAL_SCREENWIDTH)
	{
		memcpy(Ycnt + startoffset, Ycnt, viewwidth - startX);
	}
}

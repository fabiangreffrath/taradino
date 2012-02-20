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

#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifdef DOS
#include <malloc.h>
#include <dos.h>
#include <conio.h>
#include <io.h>
#endif

#include <stdlib.h>
#include <sys/stat.h>
#include "modexlib.h"
//MED
#include "memcheck.h"
#include "rt_util.h"


// GLOBAL VARIABLES


int    linewidth;
int    ylookup[MAXSCREENHEIGHT];
int    page1start;
int    page2start;
int    page3start;
int    screensize;
unsigned bufferofs;
unsigned displayofs;
boolean graphicsmode=false;

#ifdef DOS

/*
====================
=
= GraphicsMode
=
====================
*/
void GraphicsMode ( void )
{
union REGS regs;

regs.w.ax = 0x13;
int386(0x10,&regs,&regs);
graphicsmode=true;
}

/*
====================
=
= SetTextMode
=
====================
*/
void SetTextMode ( void )
{

union REGS regs;

regs.w.ax = 0x03;
int386(0x10,&regs,&regs);
graphicsmode=false;

}

/*
====================
=
= TurnOffTextCursor
=
====================
*/
void TurnOffTextCursor ( void )
{

union REGS regs;

regs.w.ax = 0x0100;
regs.w.cx = 0x2000;
int386(0x10,&regs,&regs);

}

#if 0
/*
====================
=
= TurnOnTextCursor
=
====================
*/
void TurnOnTextCursor ( void )
{

union REGS regs;

regs.w.ax = 0x03;
int386(0x10,&regs,&regs);

}
#endif

/*
====================
=
= WaitVBL
=
====================
*/
void WaitVBL( void )
{
   unsigned char i;

   i=inp(0x03da);
   while ((i&8)==0)
      i=inp(0x03da);
   while ((i&8)==1)
      i=inp(0x03da);
}


/*
====================
=
= VL_SetLineWidth
=
= Line witdh is in WORDS, 40 words is normal width for vgaplanegr
=
====================
*/

void VL_SetLineWidth (unsigned width)
{
   int i,offset;

//
// set wide virtual screen
//
   outpw (CRTC_INDEX,CRTC_OFFSET+width*256);

//
// set up lookup tables
//
   linewidth = width*2;

   offset = 0;

   for (i=0;i<MAXSCANLINES;i++)
      {
      ylookup[i]=offset;
      offset += linewidth;
      }
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode ( void )
{
    GraphicsMode();
    VL_DePlaneVGA ();
    VL_SetLineWidth (48);
    screensize=208*SCREENBWIDE;
    page1start=0xa0200;
    page2start=0xa0200+screensize;
    page3start=0xa0200+(2u*screensize);
    displayofs = page1start;
    bufferofs = page2start;
    XFlipPage ();
}

/*
=======================
=
= VL_CopyPlanarPage
=
=======================
*/
void VL_CopyPlanarPage ( byte * src, byte * dest )
{
   int plane;

   for (plane=0;plane<4;plane++)
      {
      VGAREADMAP(plane);
      VGAWRITEMAP(plane);
      memcpy(dest,src,screensize);
      }
}

/*
=======================
=
= VL_CopyPlanarPageToMemory
=
=======================
*/
void VL_CopyPlanarPageToMemory ( byte * src, byte * dest )
{
   byte * ptr;
   int plane,a,b;

   for (plane=0;plane<4;plane++)
      {
      ptr=dest+plane;
      VGAREADMAP(plane);
      for (a=0;a<200;a++)
         for (b=0;b<80;b++,ptr+=4)
            *(ptr)=*(src+(a*linewidth)+b);
      }
}

/*
=======================
=
= VL_CopyBufferToAll
=
=======================
*/
void VL_CopyBufferToAll ( unsigned buffer )
{
   int plane;

   for (plane=0;plane<4;plane++)
      {
      VGAREADMAP(plane);
      VGAWRITEMAP(plane);
      if (page1start!=buffer)
         memcpy((byte *)page1start,(byte *)buffer,screensize);
      if (page2start!=buffer)
         memcpy((byte *)page2start,(byte *)buffer,screensize);
      if (page3start!=buffer)
         memcpy((byte *)page3start,(byte *)buffer,screensize);
      }
}

/*
=======================
=
= VL_CopyDisplayToHidden
=
=======================
*/
void VL_CopyDisplayToHidden ( void )
{
   VL_CopyBufferToAll ( displayofs );
}

/*
=================
=
= VL_ClearBuffer
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearBuffer (unsigned buf, byte color)
{
  VGAMAPMASK(15);
  memset((byte *)buf,color,screensize);
}

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo (byte color)
{
  VGAMAPMASK(15);
  memset((byte *)(0xa000<<4),color,0x10000);
}

/*
=================
=
= VL_DePlaneVGA
=
=================
*/

void VL_DePlaneVGA (void)
{

//
// change CPU addressing to non linear mode
//

//
// turn off chain 4 and odd/even
//
        outp (SC_INDEX,SC_MEMMODE);
        outp (SC_DATA,(inp(SC_DATA)&~8)|4);

        outp (SC_INDEX,SC_MAPMASK);         // leave this set throughout

//
// turn off odd/even and set write mode 0
//
        outp (GC_INDEX,GC_MODE);
        outp (GC_DATA,inp(GC_DATA)&~0x13);

//
// turn off chain
//
        outp (GC_INDEX,GC_MISCELLANEOUS);
        outp (GC_DATA,inp(GC_DATA)&~2);

//
// clear the entire buffer space, because int 10h only did 16 k / plane
//
        VL_ClearVideo (0);

//
// change CRTC scanning from doubleword to byte mode, allowing >64k scans
//
        outp (CRTC_INDEX,CRTC_UNDERLINE);
        outp (CRTC_DATA,inp(CRTC_DATA)&~0x40);

        outp (CRTC_INDEX,CRTC_MODE);
        outp (CRTC_DATA,inp(CRTC_DATA)|0x40);
}


/*
=================
=
= XFlipPage
=
=================
*/

void XFlipPage ( void )
{
   displayofs=bufferofs;

//   _disable();

   outp(CRTC_INDEX,CRTC_STARTHIGH);
   outp(CRTC_DATA,((displayofs&0x0000ffff)>>8));

//   _enable();

   bufferofs += screensize;
   if (bufferofs > page3start)
      bufferofs = page1start;
}

#else

#include "SDL.h"

/* rt_def.h isn't included, so I just put this here... */
#if !defined(ANSIESC)
#define STUB_FUNCTION fprintf(stderr,"STUB: %s at " __FILE__ ", line %d, thread %d\n",__FUNCTION__,__LINE__,getpid())
#else
#define STUB_FUNCTION
#endif

/*
====================
=
= GraphicsMode
=
====================
*/
static SDL_Surface *sdl_surface = NULL;
static SDL_Surface *sdl_backbuf = NULL;

void GraphicsMode ( void )
{
    Uint32 flags = 0;

	if (SDL_InitSubSystem (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
	    Error ("Could not initialize SDL\n");
	}

    #if defined(PLATFORM_WIN32) || defined(PLATFORM_MACOSX)
        // FIXME: remove this.  --ryan.
        flags = SDL_FULLSCREEN;
        SDL_WM_GrabInput(SDL_GRAB_ON);
    #endif

    SDL_WM_SetCaption ("Rise of the Triad", "ROTT");
    SDL_ShowCursor (0);
    sdl_surface = SDL_SetVideoMode (320, 200, 8, flags);
    
	if (sdl_surface == NULL)
	{
		Error ("Could not set video mode\n");
	}
}

/*
====================
=
= SetTextMode
=
====================
*/
void SetTextMode ( void )
{
	if (SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
		if (sdl_surface != NULL) {
			SDL_FreeSurface(sdl_surface);
	
			sdl_surface = NULL;
		}
		
		SDL_QuitSubSystem (SDL_INIT_VIDEO);
	}
}

/*
====================
=
= TurnOffTextCursor
=
====================
*/
void TurnOffTextCursor ( void )
{
}

/*
====================
=
= WaitVBL
=
====================
*/
void WaitVBL( void )
{
	SDL_Delay (16667/1000);
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode ( void )
{
   int i,offset;

    GraphicsMode();

//
// set up lookup tables
//
   linewidth = 320;

   offset = 0;

   for (i=0;i<MAXSCANLINES;i++)
      {
      ylookup[i]=offset;
      offset += linewidth;
      }

    screensize=MAXSCREENHEIGHT*MAXSCREENWIDTH;

    page1start=(int)sdl_surface->pixels;
    page2start=(int)sdl_surface->pixels;
    page3start=(int)sdl_surface->pixels;
    displayofs = page1start;
    bufferofs = page2start;
    XFlipPage ();
}

/*
=======================
=
= VL_CopyPlanarPage
=
=======================
*/
void VL_CopyPlanarPage ( byte * src, byte * dest )
{
#ifdef DOS
   int plane;

   for (plane=0;plane<4;plane++)
      {
      VGAREADMAP(plane);
      VGAWRITEMAP(plane);
      memcpy(dest,src,screensize);
      }
#else
      memcpy(dest,src,screensize);
#endif
}

/*
=======================
=
= VL_CopyPlanarPageToMemory
=
=======================
*/
void VL_CopyPlanarPageToMemory ( byte * src, byte * dest )
{
#ifdef DOS
   byte * ptr;
   int plane,a,b;

   for (plane=0;plane<4;plane++)
      {
      ptr=dest+plane;
      VGAREADMAP(plane);
      for (a=0;a<200;a++)
         for (b=0;b<80;b++,ptr+=4)
            *(ptr)=*(src+(a*linewidth)+b);
      }
#else
      memcpy(dest,src,screensize);
#endif
}

/*
=======================
=
= VL_CopyBufferToAll
=
=======================
*/
void VL_CopyBufferToAll ( unsigned buffer )
{
#ifdef DOS
   int plane;

   for (plane=0;plane<4;plane++)
      {
      VGAREADMAP(plane);
      VGAWRITEMAP(plane);
      if (page1start!=buffer)
         memcpy((byte *)page1start,(byte *)buffer,screensize);
      if (page2start!=buffer)
         memcpy((byte *)page2start,(byte *)buffer,screensize);
      if (page3start!=buffer)
         memcpy((byte *)page3start,(byte *)buffer,screensize);
      }
#endif
}

/*
=======================
=
= VL_CopyDisplayToHidden
=
=======================
*/
void VL_CopyDisplayToHidden ( void )
{
   VL_CopyBufferToAll ( displayofs );
}

/*
=================
=
= VL_ClearBuffer
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearBuffer (unsigned buf, byte color)
{
#ifdef DOS
  VGAMAPMASK(15);
  memset((byte *)buf,color,screensize);
#else
  memset((byte *)buf,color,screensize);
#endif
}

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo (byte color)
{
#ifdef DOS
  VGAMAPMASK(15);
  memset((byte *)(0xa000<<4),color,0x10000);
#else
  memset (sdl_surface->pixels, color, MAXSCREENWIDTH*MAXSCREENHEIGHT);
#endif
}

/*
=================
=
= VL_DePlaneVGA
=
=================
*/

void VL_DePlaneVGA (void)
{
}


/* C version of rt_vh_a.asm */

void VH_UpdateScreen (void)
{
	SDL_UpdateRect (SDL_GetVideoSurface (), 0, 0, 0, 0);
}

/*
=================
=
= XFlipPage
=
=================
*/

void XFlipPage ( void )
{
#ifdef DOS
   displayofs=bufferofs;

//   _disable();

   outp(CRTC_INDEX,CRTC_STARTHIGH);
   outp(CRTC_DATA,((displayofs&0x0000ffff)>>8));

//   _enable();

   bufferofs += screensize;
   if (bufferofs > page3start)
      bufferofs = page1start;
#else
   SDL_UpdateRect (sdl_surface, 0, 0, 0, 0);
#endif
}

#endif

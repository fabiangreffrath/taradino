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

#include <stdlib.h>
#include <sys/stat.h>
#include "modexlib.h"
#include "rt_util.h"
#include "rt_net.h" // for GamePaused

#include "isr.h" // for VBLCOUNTER

static void StretchMemPicture ();
// GLOBAL VARIABLES

boolean StretchScreen=0;//bna++
extern boolean iG_aimCross;
extern boolean sdl_fullscreen;
extern int iG_X_center;
extern int iG_Y_center;
byte  *iG_buf_center;
  
int    linewidth;
//int    ylookup[MAXSCREENHEIGHT];
int    ylookup[600];//just set to max res
byte  *SCREEN_BUFFER;
int    screensize;
byte  *bufferofs;
byte  *displayofs;
boolean graphicsmode=false;
byte  *bufofsTopLimit;
byte  *bufofsBottomLimit;

void DrawCenterAim ();

#include "SDL.h"

/*
====================
=
= GraphicsMode
=
====================
*/
SDL_Surface *sdl_surface = NULL;
SDL_Surface *unstretch_sdl_surface = NULL;
static SDL_Surface *unstretch_sdl_argbbuffer = NULL;
static SDL_Texture *unstretch_sdl_texture = NULL;

static SDL_Window *screen = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Surface *argbbuffer = NULL;
static SDL_Texture *texture = NULL;

SDL_Window *VL_GetVideoWindow (void)
{
	return screen;
}

SDL_Surface *VL_GetVideoSurface (void)
{
	return sdl_surface;
}

int VL_SaveBMP (const char *file)
{
    return SDL_SaveBMP(sdl_surface, file);
}

void SetShowCursor(int show)
{
	SDL_SetRelativeMouseMode(!show);
	SDL_GetRelativeMouseState(NULL, NULL);
}

void GraphicsMode ( void )
{
	uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

	if (SDL_InitSubSystem (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		Error ("Could not initialize SDL\n");
	}

	if (sdl_fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	screen = SDL_CreateWindow(NULL,
	                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                          iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT,
	                          flags);
	SDL_SetWindowMinimumSize(screen, iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT);
	SDL_SetWindowTitle(screen, PACKAGE_STRING);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
	}
	SDL_RenderSetLogicalSize(renderer, 640, 480);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	sdl_surface = SDL_CreateRGBSurface(0,
	                                   iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT, 8,
	                                   0, 0, 0, 0);
	SDL_FillRect(sdl_surface, NULL, 0);

	argbbuffer = SDL_CreateRGBSurfaceWithFormatFrom(NULL, iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT, 0, 0, SDL_GetWindowPixelFormat(screen));

	texture = SDL_CreateTexture(renderer,
	                            SDL_GetWindowPixelFormat(screen),
	                            SDL_TEXTUREACCESS_STREAMING,
	                            iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT);

	unstretch_sdl_surface = SDL_CreateRGBSurface(0, 320, 200, 8, 0, 0, 0, 0);
	SDL_SetColorKey(unstretch_sdl_surface, SDL_TRUE, 0);
	unstretch_sdl_texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(screen), SDL_TEXTUREACCESS_STREAMING, iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT);
	unstretch_sdl_argbbuffer = SDL_CreateRGBSurfaceWithFormatFrom(NULL, iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT, 0, 0, SDL_GetWindowPixelFormat(screen));

	SetShowCursor(!sdl_fullscreen);
}

void ToggleFullScreen (void)
{
	unsigned int flags = 0;

	sdl_fullscreen = !sdl_fullscreen;

	if (sdl_fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	SDL_SetWindowFullscreen(screen, flags);
	SetShowCursor(!sdl_fullscreen);
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
		if (sdl_surface) SDL_FreeSurface(sdl_surface);
		if (unstretch_sdl_surface) SDL_FreeSurface(unstretch_sdl_surface);
		if (argbbuffer) SDL_FreeSurface(argbbuffer);
		if (texture) SDL_DestroyTexture(texture);
		if (renderer) SDL_DestroyRenderer(renderer);
		if (screen) SDL_DestroyWindow(screen);

		sdl_surface = NULL;
		unstretch_sdl_surface = NULL;
		argbbuffer = NULL;
		texture = NULL;
		renderer = NULL;
		screen = NULL;

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

static Uint64 next_time = 0;

static Uint64 time_left(void)
{
	Uint64 now = SDL_GetTicks();

	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void WaitVBL( void )
{
	// was:
	// SDL_Delay (16667/1000);

	if (next_time == 0)
		next_time = SDL_GetTicks() + VBLCOUNTER;

	SDL_Delay(time_left());
	next_time += VBLCOUNTER;
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
//bna--   linewidth = 320;
   linewidth = iGLOBAL_SCREENWIDTH;

   offset = 0;

   for (i=0;i<iGLOBAL_SCREENHEIGHT;i++)
      {
      ylookup[i]=offset;
      offset += linewidth;
      }

//    screensize=MAXSCREENHEIGHT*MAXSCREENWIDTH;
    screensize=iGLOBAL_SCREENHEIGHT*iGLOBAL_SCREENWIDTH;

    SCREEN_BUFFER = displayofs = bufferofs = sdl_surface->pixels;

	iG_X_center = iGLOBAL_SCREENWIDTH / 2;
	iG_Y_center = (iGLOBAL_SCREENHEIGHT / 2)+10 ;//+10 = move aim down a bit

	iG_buf_center = bufferofs + (screensize/2);//(iG_Y_center*iGLOBAL_SCREENWIDTH);//+iG_X_center;

	bufofsTopLimit =  bufferofs + screensize - iGLOBAL_SCREENWIDTH;
	bufofsBottomLimit = bufferofs + iGLOBAL_SCREENWIDTH;

    // start stretched
    EnableScreenStretch();
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
      memcpy(dest,src,screensize);
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
      memcpy(dest,src,screensize);
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

void VL_ClearBuffer (byte *buf, byte color)
{
	SDL_FillRect(unstretch_sdl_surface, NULL, 0);
	SDL_FillRect(sdl_surface, NULL, color);
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
	SDL_FillRect(unstretch_sdl_surface, NULL, 0);
	SDL_FillRect(sdl_surface, NULL, color);
}

/* C version of rt_vh_a.asm */

void VH_UpdateScreen (void)
{ 	
	if (StretchScreen){//bna++
		StretchMemPicture ();
	}else{
		DrawCenterAim ();
	}

	if (SDL_LockTexture(texture, NULL, &argbbuffer->pixels, &argbbuffer->pitch) == 0)
	{
		SDL_BlitSurface(sdl_surface, NULL, argbbuffer, NULL);
		SDL_UnlockTexture(texture);
	}

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (StretchScreen)
		SDL_RenderCopy(renderer, unstretch_sdl_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
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
	if (StretchScreen){//bna++
		StretchMemPicture ();
	}else{
		DrawCenterAim ();
	}

	if (SDL_LockTexture(texture, NULL, &argbbuffer->pixels, &argbbuffer->pitch) == 0)
	{
		SDL_BlitSurface(sdl_surface, NULL, argbbuffer, NULL);
		SDL_UnlockTexture(texture);
	}

   SDL_RenderClear(renderer);
   SDL_RenderCopy(renderer, texture, NULL, NULL);
   if (StretchScreen)
       SDL_RenderCopy(renderer, unstretch_sdl_texture, NULL, NULL);
   SDL_RenderPresent(renderer);
}

void EnableScreenStretch(void)
{
   if (iGLOBAL_SCREENWIDTH <= 320 || StretchScreen) return;
	
   displayofs = (byte *)unstretch_sdl_surface->pixels +
	(displayofs - (byte *)sdl_surface->pixels);
   SCREEN_BUFFER = bufferofs = unstretch_sdl_surface->pixels;
   StretchScreen = 1;	
}

void DisableScreenStretch(void)
{
   if (iGLOBAL_SCREENWIDTH <= 320 || !StretchScreen) return;
	
   displayofs = (byte *)sdl_surface->pixels +
	(displayofs - (byte *)unstretch_sdl_surface->pixels);
   SCREEN_BUFFER = bufferofs = sdl_surface->pixels;
   StretchScreen = 0;
}


// bna section -------------------------------------------
static void StretchMemPicture ()
{
	if (SDL_LockTexture(unstretch_sdl_texture, NULL, &unstretch_sdl_argbbuffer->pixels, &unstretch_sdl_argbbuffer->pitch) == 0)
	{
		SDL_BlitSurface(unstretch_sdl_surface, NULL, unstretch_sdl_argbbuffer, NULL);
		SDL_UnlockTexture(unstretch_sdl_texture);
	}
}

// bna function added start
extern	boolean ingame;
int		iG_playerTilt;

void DrawCenterAim ()
{
	int x;

	int percenthealth = (locplayerstate->health * 10) / MaxHitpointsForCharacter(locplayerstate);
	int color = percenthealth < 3 ? egacolor[RED] : percenthealth < 4 ? egacolor[YELLOW] : egacolor[GREEN];

	if (iG_aimCross && !GamePaused){
		if (( ingame == true )&&(iGLOBAL_SCREENWIDTH>320)){
			  if ((iG_playerTilt <0 )||(iG_playerTilt >iGLOBAL_SCREENHEIGHT/2)){
					iG_playerTilt = -(2048 - iG_playerTilt);
			  }
			  if (iGLOBAL_SCREENWIDTH == 640){ x = iG_playerTilt;iG_playerTilt=x/2; }
			  iG_buf_center = bufferofs + ((iG_Y_center-iG_playerTilt)*iGLOBAL_SCREENWIDTH);//+iG_X_center;

			  for (x=iG_X_center-10;x<=iG_X_center-4;x++){
				  if ((iG_buf_center+x < bufofsTopLimit)&&(iG_buf_center+x > bufofsBottomLimit)){
					 *(iG_buf_center+x) = color;
				  }
			  }
			  for (x=iG_X_center+4;x<=iG_X_center+10;x++){
				  if ((iG_buf_center+x < bufofsTopLimit)&&(iG_buf_center+x > bufofsBottomLimit)){
					 *(iG_buf_center+x) = color;
				  }
			  }
			  for (x=10;x>=4;x--){
				  if (((iG_buf_center-(x*iGLOBAL_SCREENWIDTH)+iG_X_center) < bufofsTopLimit)&&((iG_buf_center-(x*iGLOBAL_SCREENWIDTH)+iG_X_center) > bufofsBottomLimit)){
					 *(iG_buf_center-(x*iGLOBAL_SCREENWIDTH)+iG_X_center) = color;
				  }
			  }
			  for (x=4;x<=10;x++){
				  if (((iG_buf_center+(x*iGLOBAL_SCREENWIDTH)+iG_X_center) < bufofsTopLimit)&&((iG_buf_center+(x*iGLOBAL_SCREENWIDTH)+iG_X_center) > bufofsBottomLimit)){
					 *(iG_buf_center+(x*iGLOBAL_SCREENWIDTH)+iG_X_center) = color;
				  }
			  }
		}
	}
}
// bna function added end




// bna section -------------------------------------------




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

#include "rt_def.h"

#include "SDL.h"

#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "watcom.h"
#include "_rt_util.h"
#include "rt_util.h"
#include "isr.h"
#include "z_zone.h"
#include "rt_dr_a.h"
#include "rt_in.h"
#include "rt_main.h"
#include "scriplib.h"
#include "rt_menu.h"
#include "rt_playr.h"
#include "version.h"
#include "develop.h"
#include "rt_vid.h"
#include "rt_view.h"
#include "modexlib.h"
#include "rt_cfg.h"
//MED
#include "memcheck.h"

int    egacolor[16];
byte   *  origpal;
FILE   *  errout;
FILE   *  debugout;
FILE   *  mapdebugout;

static boolean SoftErrorStarted=false;
static boolean DebugStarted=false;
static boolean MapDebugStarted=false;

extern SDL_Surface *VL_GetVideoSurface (void);

static unsigned char egargb[48]={ 0x00,0x00,0x00,
									 0x00,0x00,0xab,
                            0x00,0xab,0x00,
                            0x00,0xab,0xab,
                            0xab,0x00,0x00,
                            0xab,0x00,0xab,
                            0xab,0x57,0x00,
                            0xab,0xab,0xab,
                            0x57,0x57,0x57,
                            0x57,0x57,0xff,
                            0x57,0xff,0x57,
                            0x57,0xff,0xff,
                            0xff,0x57,0x57,
                            0xff,0x57,0xff,
                            0xff,0xff,0x57,
									 0xff,0xff,0xff};

extern const byte * ROTT_ERR;

#if (DEVELOPMENT == 1)
int TotalStaticMemory=0;
#endif

#define SWAP(a,b) \
   {              \
   a=(a)^(b);     \
   b=(a)^(b);     \
   a=(a)^(b);     \
   }              \

//******************************************************************************
//
// FindDistance
//
//******************************************************************************

int FindDistance(int ix, int iy)
{
  int   t;

  ix= abs(ix);        /* absolute values */
  iy= abs(iy);

  if (ix<iy)
     SWAP(ix,iy);

  t = iy + (iy>>1);

  return (ix - (ix>>5) - (ix>>7)  + (t>>2) + (t>>6));
}


//******************************************************************************
//
// Find_3D_Distance
//
//******************************************************************************

int Find_3D_Distance(int ix, int iy, int iz)
   {
   int   t;

   ix= abs(ix);           /* absolute values */
   iy= abs(iy);
   iz= abs(iz);

   if (ix<iy)
     SWAP(ix,iy);

   if (ix<iz)
     SWAP(ix,iz);

   t = iy + iz;

   return (ix - (ix>>4) + (t>>2) + (t>>3));
   }

//******************************************************************************
//
// atan2_appx
//
//******************************************************************************

int atan2_appx(int dx, int dy)
{int absdx, absdy;
 fixed angle;
 fixed ratio;


 if (!(dx||dy))
  return 0;
 absdx = abs(dx);
 absdy = abs(dy);
 if (absdx >= absdy)
  ratio = FixedDiv2(absdy,absdx);
 else
  ratio = FixedDiv2(absdx,absdy);

 if (dx >= 0)
  {if (dy >= 0)
	 {if (absdx >= absdy)
		angle = ratio;	    // 1st octant
	  else
		angle = (2<<16) - ratio; // 2nd octant
	 }
	else
	 {if (absdx >= absdy)
		angle = (8<<16) - ratio; // 8th octant
	  else
		angle = (6<<16) + ratio; // 7th octant
	 }
  }
 else
  {if (dy >= 0)
	 {if (absdx >= absdy)
		angle = (4<<16) - ratio; // 4th octant
	  else
		angle = (2<<16) + ratio; // 3rd octant
	 }
	else
	 {if (absdx >= absdy)
		angle = (4<<16) + ratio; // 5th octant
	  else
		angle = (6<<16) - ratio; // 6th octant
	 }
  }

 return (((int)FixedMul(angle,ANGLESDIV8))&(FINEANGLES-1));
}



//******************************************************************************
//
// StringsNotEqual
//
//******************************************************************************
boolean StringsNotEqual (char * s1, char * s2, int length)
{
   int i;

   for (i=0;i<length;i++)
      if (s1[i]!=s2[i])
         return true;
   return false;
}



void markgetch( void )
{
   int done;
   int i;

   done=0;
   while (done==0)
      {
      IN_UpdateKeyboard ();
      for (i=0;i<127;i++)
         if (Keyboard[i]==1)
            done=i;
      }
   while (Keyboard[done])
      IN_UpdateKeyboard ();
}

/*
====================
=
= FindEGAColors
=
====================
*/

void FindEGAColors ( void )
{
   int i;

	for (i=0;i<16;i++)
		egacolor[i]=BestColor((int)egargb[i*3],(int)egargb[i*3+1],(int)egargb[i*3+2],origpal);
}

//===========================================================================


byte BestColor (int r, int g, int b, byte *palette)
{
	int	i;
	long	dr, dg, db;
	long	bestdistortion, distortion;
	int	bestcolor;
	byte	*pal;

//
// let any color go to 0 as a last resort
//
   bestdistortion = ( (long)WeightR*r*r + (long)WeightG*g*g + (long)WeightB*b*b )*2;
	bestcolor = 0;

	pal = &palette[0];
	for (i=0 ; i<= 255 ; i++,pal+=3)
	{
		dr = r - (int)pal[0];
		dg = g - (int)pal[1];
		db = b - (int)pal[2];
      distortion = WeightR*dr*dr + WeightG*dg*dg + WeightB*db*db;
		if (distortion < bestdistortion)
		{
			if (!distortion)
				return i;		// perfect match

			bestdistortion = distortion;
			bestcolor = i;
		}
	}

	return bestcolor;
}

void ClearGraphicsScreen( void )
{
VL_ClearVideo(0);
}

void ClearBuffer( char * buf, int size )
{
        memset(buf,0,size);
}

/*
=============================================================================

						MISC FUNCTIONS

=============================================================================
*/

/*
=================
=
= Error
=
= For abnormal program terminations
=
=================
*/

void Error (char *error, ...)
{
   char msgbuf[300];
	va_list	argptr;
   int size;
   char * sptr;
   int level;
   static int inerror = 0;


   inerror++;
   if (inerror > 1)
      abort();


	SetTextMode ();
   memset (msgbuf, 0, 300);

	va_start (argptr, error);
   vsprintf (&msgbuf[0], error, argptr);
	va_end (argptr);

   scriptbuffer = &msgbuf[0];
	size = strlen (msgbuf);

	sptr = script_p = scriptbuffer;
	scriptend_p = script_p + size;
	scriptline = 1;
	endofscript = false;
	tokenready = false;

   px = ERRORCOL;
   py = ERRORROW;

   GetToken (true);
   while (!endofscript)
   {
      if ((script_p - sptr) >= 60)
      {
         px = ERRORCOL;
         py++;
         sptr = script_p;
      }

      UL_printf (token);
      px++;                //SPACE
      GetToken (true);
   }

   if (player!=NULL)
      {
      printf ("Player X     = %lx\n", (long int)player->x);
      printf ("Player Y     = %lx\n", (long int)player->y);
      printf ("Player Angle = %lx\n\n", (long int)player->angle);
      }
   printf ("Episode      = %ld\n", (long int)gamestate.episode);

   if (gamestate.episode > 1)
      level = (gamestate.mapon+1) - ((gamestate.episode-1) << 3);
   else
      level = gamestate.mapon+1;

   printf ("Area         = %ld\n", (long int)level);

   ShutDown();	// DDOI - moved this so that it doesn't try to access player
   		// which is freed by this function.

   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                            PACKAGE_STRING, msgbuf, NULL);
   SDL_Quit();

   exit (1);
}

//#if (SOFTERROR==1)

/*
=================
=
= SoftwareError
=
=================
*/
void SoftwareError (char *error, ...)
{
	va_list	argptr;

	if (SoftErrorStarted==false)
      return;
	va_start (argptr, error);
   vfprintf (errout, error, argptr);
	va_end (argptr);
}

//#endif


//#if (DEBUG == 1)

/*
=================
=
= DebugError
=
=================
*/
void DebugError (char *error, ...)
{
	va_list	argptr;

   if (DebugStarted==false)
      return;
	va_start (argptr, error);
   vfprintf (debugout, error, argptr);
	va_end (argptr);
}

//#endif

/*
=================
=
= OpenSoftError
=
=================
*/
void OpenSoftError ( void )
{
  errout = fopen(SOFTERRORFILE,"wt+");
  SoftErrorStarted=true;
}

/*
=================
=
= MapDebug
=
=================
*/
void MapDebug (char *error, ...)
{
	va_list	argptr;

   if (MapDebugStarted==false)
      return;
	va_start (argptr, error);
   vfprintf (mapdebugout, error, argptr);
	va_end (argptr);
}

/*
=================
=
= OpenMapDebug
=
=================
*/
void OpenMapDebug ( void )
{
  char filename[ 128 ];

  if (MapDebugStarted==true)
     return;
  GetPathFromEnvironment( filename, ApogeePath, MAPDEBUGFILE );
  mapdebugout = fopen(filename,"wt+");
  MapDebugStarted=true;
}


/*
=================
=
= StartupSoftError
=
=================
*/
void StartupSoftError ( void )
{
#if (DEBUG == 1)
  if (DebugStarted==false)
     {
     debugout = fopen(DEBUGFILE,"wt+");
     DebugStarted=true;
     }
#endif
#if (SOFTERROR == 1)
  if (SoftErrorStarted==false)
     OpenSoftError();
#endif
}

/*
=================
=
= ShutdownSoftError
=
=================
*/
void ShutdownSoftError ( void )
{
  if (DebugStarted==true)
     {
     fclose(debugout);
     DebugStarted=false;
     }
  if (SoftErrorStarted==true)
     {
     fclose(errout);
     SoftErrorStarted=false;
     }
  if (MapDebugStarted==true)
     {
     fclose(mapdebugout);
     MapDebugStarted=false;
     }
}


/*
=================
=
= CheckParm
=
= Checks for the given parameter in the program's command line arguments
=
= Returns the argument number (1 to argc-1) or 0 if not present
=
=================
*/

int CheckParm (char *check)
{
	int		i;
	char	*parm;

	for (i = 1;i<_argc;i++)
	{
		parm = _argv[i];
		if ( !isalpha(*parm) )	// skip - / \ etc.. in front of parm
         {
         parm++;
         if (!*parm)
				continue;		// parm was only one char
         }

		if ( !_fstricmp(check,parm) )
			return i;
	}

	return 0;
}



int SafeOpenAppend (char *_filename)
{
	int	handle;
    char filename[MAX_PATH];
    strncpy(filename, _filename, sizeof (filename));
    filename[sizeof (filename) - 1] = '\0';
    FixFilePath(filename);

	handle = open(filename,O_RDWR | O_BINARY | O_CREAT | O_APPEND
	, S_IREAD | S_IWRITE);

	if (handle == -1)
		Error ("Error opening for append %s: %s",filename,strerror(errno));

	return handle;
}

int SafeOpenWrite (char *_filename)
{
	int	handle;
    char filename[MAX_PATH];
    strncpy(filename, _filename, sizeof (filename));
    filename[sizeof (filename) - 1] = '\0';
    FixFilePath(filename);

	handle = open(filename,O_RDWR | O_BINARY | O_CREAT | O_TRUNC
	, S_IREAD | S_IWRITE);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
}

int SafeOpenRead (char *_filename)
{
	int	handle;
    char filename[MAX_PATH];
    strncpy(filename, _filename, sizeof (filename));
    filename[sizeof (filename) - 1] = '\0';
    FixFilePath(filename);

	handle = open(filename,O_RDONLY | O_BINARY);

	if (handle == -1)
		Error ("Error opening %s: %s",filename,strerror(errno));

	return handle;
}


void SafeRead (int handle, void *buffer, long count)
{
	unsigned	iocount;

	while (count)
	{
		iocount = count > 0x8000 ? 0x8000 : count;
		if (read (handle,buffer,iocount) != (int)iocount)
			Error ("File read failure reading %ld bytes",count);
		buffer = (void *)( (byte *)buffer + iocount );
		count -= iocount;
	}
}


void SafeWrite (int handle, void *buffer, long count)
{
	unsigned	iocount;

	while (count)
	{
		iocount = count > 0x8000 ? 0x8000 : count;
		if (write (handle,buffer,iocount) != (int)iocount)
			Error ("File write failure writing %ld bytes",count);
		buffer = (void *)( (byte *)buffer + iocount );
		count -= iocount;
	}
}

void SafeWriteString (int handle, char * buffer)
{
	unsigned	iocount;

   iocount=strlen(buffer);
	if (write (handle,buffer,iocount) != (int)iocount)
			Error ("File write string failure writing %s\n",buffer);
}

void *SafeMalloc (long size)
{
	void *ptr;

   if (zonememorystarted==false)
      Error("Called SafeMalloc without starting zone memory\n");
	ptr = Z_Malloc (size,PU_STATIC,NULL);

	if (!ptr)
      Error ("SafeMalloc failure for %lu bytes",size);

	return ptr;
}

void *SafeLevelMalloc (long size)
{
	void *ptr;

   if (zonememorystarted==false)
      Error("Called SafeLevelMalloc without starting zone memory\n");
   ptr = Z_LevelMalloc (size,PU_STATIC,NULL);

	if (!ptr)
      Error ("SafeLevelMalloc failure for %lu bytes",size);

	return ptr;
}

void SafeFree (void * ptr)
{
   if ( ptr == NULL )
      Error ("SafeFree : Tried to free a freed pointer\n");

	Z_Free (ptr);
}

/*
==============
=
= LoadFile
=
==============
*/

long	LoadFile (char *filename, void **bufferptr)
{
	int		handle;
	long	length;

	handle = SafeOpenRead (filename);
	length = filelength (handle);
	*bufferptr = SafeMalloc (length);
	SafeRead (handle,*bufferptr, length);
	close (handle);
	return length;
}


/*
==============
=
= SaveFile
=
==============
*/

void	SaveFile (char *filename, void *buffer, long count)
{
	int		handle;

	handle = SafeOpenWrite (filename);
	SafeWrite (handle, buffer, count);
	close (handle);
}


void FixFilePath(char *filename)
{
#ifndef _WIN32
    char *ptr;
    char *lastsep = filename;

    if ((!filename) || (*filename == '\0'))
        return;

    if (access(filename, F_OK) == 0)  /* File exists; we're good to go. */
        return;

    for (ptr = filename; 1; ptr++)
    {
        if (*ptr == '\\')
            *ptr = PATH_SEP_CHAR;

        if ((*ptr == PATH_SEP_CHAR) || (*ptr == '\0'))
        {
            char pch = *ptr;
            struct dirent *dent = NULL;
            DIR *dir;

            if ((pch == PATH_SEP_CHAR) && (*(ptr + 1) == '\0'))
                return; /* eos is pathsep; we're done. */

            if (lastsep == ptr)
                continue;  /* absolute path; skip to next one. */

            *ptr = '\0';
            if (lastsep == filename) {
                dir = opendir((*lastsep == PATH_SEP_CHAR) ? ROOTDIR : CURDIR);
                
                if (*lastsep == PATH_SEP_CHAR) {
                    lastsep++;
                }
            } 
            else
            {
                *lastsep = '\0';
                dir = opendir(filename);
                *lastsep = PATH_SEP_CHAR;
                lastsep++;
            }

            if (dir == NULL)
            {
                *ptr = PATH_SEP_CHAR;
                return;  /* maybe dir doesn't exist? give up. */
            }

            while ((dent = readdir(dir)) != NULL)
            {
                if (strcasecmp(dent->d_name, lastsep) == 0)
                {
                    /* found match; replace it. */
                    strcpy(lastsep, dent->d_name);
                    break;
                }
            }

            closedir(dir);
            *ptr = pch;
            lastsep = ptr;

            if (dent == NULL)
                return;  /* no match. oh well. */

            if (pch == '\0')  /* eos? */
                return;
        }
    }
#endif
}

void _dos_getdate(struct dosdate_t *date)
{
	time_t curtime = time(NULL);
	struct tm *tm;
	
	if (date == NULL) {
		return;
	}
	
	memset(date, 0, sizeof(struct dosdate_t));
	
	if ((tm = localtime(&curtime)) != NULL) {
		date->day = tm->tm_mday;
		date->month = tm->tm_mon + 1;
		date->year = tm->tm_year + 1900;
		date->dayofweek = tm->tm_wday + 1;
	}
}


void GetPathFromEnvironment( char *fullname, const char *envname, const char *filename )
   {

   const char *path;
   path = envname;

   if ( path != NULL )
      {
      strcpy( fullname, path );
      if ( fullname[ strlen( fullname ) - 1 ] != PATH_SEP_CHAR )
         {
         strcat( fullname, PATH_SEP_STR );
         }
      strcat( fullname, filename );
      }
   else
      {
      strcpy( fullname, filename );
      }

      FixFilePath(fullname);
   }

void DefaultExtension (char *path, char *extension)
{
	char	*src;
//
// if path doesn't have a .EXT, append extension
// (extension should include the .)
//
	src = path + strlen(path) - 1;

	while (*src != PATH_SEP_CHAR && src != path)
	{
		if (*src == '.')
			return;			// it has an extension
		src--;
	}

	strcat (path, extension);
}

void DefaultPath (char *path, char *basepath)
{
	char	temp[128];

	if (path[0] == PATH_SEP_CHAR)
		return;							// absolute path location
	strcpy (temp,path);
	strcpy (path,basepath);
	strcat (path,temp);
}


void ExtractFileBase (char *path, char *dest)
{
	char	*src;
	int		length;

	src = path + strlen(path) - 1;

//
// back up until a \ or the start
//
	while (src != path && *(src-1) != PATH_SEP_CHAR)
		src--;

//
// copy up to eight characters
//
	memset (dest,0,8);
	length = 0;
	while (*src && *src != '.')
	{
		if (++length == 9)
			Error ("Filename base of %s >8 chars",path);
		*dest++ = toupper(*src++);
	}
}


/*
==============
=
= ParseNum / ParseHex
=
==============
*/

long ParseHex (char *hex)
{
	char	*str;
	long	num;

	num = 0;
	str = hex;

	while (*str)
	{
		num <<= 4;
		if (*str >= '0' && *str <= '9')
			num += *str-'0';
		else if (*str >= 'a' && *str <= 'f')
			num += 10 + *str-'a';
		else if (*str >= 'A' && *str <= 'F')
			num += 10 + *str-'A';
		else
			Error ("Bad hex number: %s",hex);
		str++;
	}

	return num;
}


long ParseNum (char *str)
{
	if (str[0] == '$')
		return ParseHex (str+1);
	if (str[0] == '0' && str[1] == 'x')
		return ParseHex (str+2);
	return atol (str);
}


#if (BYTE_ORDER == LITTLE_ENDIAN)
#define KeepShort IntelShort
#define SwapShort MotoShort
#define KeepLong IntelLong
#define SwapLong MotoLong
#else
#define KeepShort MotoShort
#define SwapShort IntelShort
#define KeepLong MotoLong
#define SwapLong IntelLong
#endif

short	SwapShort (short l)
{
	byte	b1,b2;

	b1 = l&255;
	b2 = (l>>8)&255;

	return (b1<<8) + b2;
}

short	KeepShort (short l)
{
	return l;
}


int	SwapLong (int l)
{
	byte	b1,b2,b3,b4;

	b1 = l&255;
	b2 = (l>>8)&255;
	b3 = (l>>16)&255;
	b4 = (l>>24)&255;

	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
}

int	KeepLong (int l)
{
	return l;
}


#undef KeepShort
#undef KeepLong
#undef SwapShort
#undef SwapLong

void SwapIntelLong(int *l)
{
    *l = IntelLong(*l);
}

void SwapIntelShort(short *s)
{
    *s = IntelShort(*s);
}

void SwapIntelLongArray(int *l, int num)
{
    while (num--) {
        SwapIntelLong(l);
        l++;
    }
}

void SwapIntelShortArray(short *s, int num)
{
    while (num--) {
        SwapIntelShort(s);
        s++;
    }
}

/*
============================================================================

						BASIC GRAPHICS

============================================================================
*/

void GetPalette(char * palette)
{
	int i;
	SDL_Palette *pal = VL_GetVideoSurface()->format->palette;
	
	for (i = 0; i < 256; i++) {
		palette[0] = pal->colors[i].r;
		palette[1] = pal->colors[i].g;
		palette[2] = pal->colors[i].b;
		
		palette += 3;
	}
}

void SetPalette ( byte * pal )
{
   VL_SetPalette (pal);
}



//******************************************************************************
//
// US_CheckParm() - checks to see if a string matches one of a set of
//    strings. The check is case insensitive. The routine returns the
//    index of the string that matched, or -1 if no matches were found
//
//******************************************************************************

int US_CheckParm (char *parm, char **strings)
{
   char  cp,cs,
         *p,*s;
   int      i;
   int      length;

   length=strlen(parm);
   while ( (!isalpha(*parm)) && (length>0)) // Skip non-alphas
      {
      length--;
      parm++;
      }

   for (i = 0;*strings && **strings;i++)
   {
      for (s = *strings++,p = parm,cs = cp = 0;cs == cp;)
      {
         cs = *s++;
         if (!cs)
            return(i);
         cp = *p++;

         if (isupper(cs))
            cs = tolower(cs);
         if (isupper(cp))
            cp = tolower(cp);
      }
   }
   return(-1);
}

/*
=============================================================================

                  PALETTE OPS

      To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/


/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette (int red, int green, int blue)
{
   SDL_Color cmap[256];
   int i;

   for (i = 0; i < 256; i++)
   {
           cmap[i].r = red << 2;
           cmap[i].g = green << 2;
           cmap[i].b = blue << 2;
   }

   SDL_SetPaletteColors (VL_GetVideoSurface()->format->palette, cmap, 0, 256);
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor  (int color, int red, int green, int blue)
{
	STUB_FUNCTION;
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor  (int color, int *red, int *green, int *blue)
{
	STUB_FUNCTION;
}

//===========================================================================

/*
=================
=
= VL_NormalizePalette
=
=================
*/

void VL_NormalizePalette (byte *palette)
{
   int   i;

   for (i = 0; i < 768; i++)
      *(palette+i)=(*(palette+i))>>2;
}


/*
=================
=
= VL_SetPalette
=
= If fast palette setting has been tested for, it is used
= -some cards don't like outsb palette setting-
=
=================
*/

void VL_SetPalette (byte *palette)
{
   SDL_Color cmap[256];
   int i;

   for (i = 0; i < 256; i++)
   {
	   cmap[i].r = gammatable[(gammaindex<<6)+(*palette++)] << 2;
	   cmap[i].g = gammatable[(gammaindex<<6)+(*palette++)] << 2;
	   cmap[i].b = gammatable[(gammaindex<<6)+(*palette++)] << 2;
   }

   SDL_SetPaletteColors (VL_GetVideoSurface()->format->palette, cmap, 0, 256);
}


//===========================================================================

/*
=================
=
= VL_GetPalette
=
= This does not use the port string instructions,
= due to some incompatabilities
=
=================
*/

void VL_GetPalette (byte *palette)
{
	int i;
	SDL_Palette *pal = VL_GetVideoSurface()->format->palette;
	
	for (i = 0; i < 256; i++) {
		palette[0] = pal->colors[i].r >> 2;
		palette[1] = pal->colors[i].g >> 2;
		palette[2] = pal->colors[i].b >> 2;
		
		palette += 3;
	}
}


/*
=================
=
= UL_DisplayMemoryError ()
=
=================
*/

void UL_DisplayMemoryError ( int memneeded )
{
	STUB_FUNCTION;
   exit (0);
}


/*
=================
=
= UL_printf
=
=================
*/

void UL_printf (char *str)
{
   printf ("%s ",str);	// Hackish but works - DDOI
}

/*
=================
=
= UL_ColorBox
=
=================
*/

void UL_ColorBox (int x, int y, int w, int h, int color)
{
}

//******************************************************************************
//
// SideOfLine
//
//******************************************************************************

int SideOfLine(int x1, int y1, int x2, int y2, int x3, int y3)
{
   int a1,b1,c1;

   /* Compute a1, b1, c1, where line joining points 1 and 2
    * is "a1 x  +  b1 y  +  c1  =  0".
    */

   a1 = y2 - y1;
   b1 = x1 - x2;
   c1 = FixedMulShift(x2,y1,16) - FixedMulShift(x1,y2,16);

   return SGN(FixedMulShift(a1,x3,16) + FixedMulShift(b1,y3,16) + c1);
}



//******************************************************************************
//
// HSORT - heap sort
//
//******************************************************************************

typedef int (*PFI)();           /* pointer to a function returning int  */
typedef void (*PFV)();           /* pointer to a function returning int  */
static PFI Comp;                        /* pointer to comparison routine                */
static PFV Switch;                        /* pointer to comparison routine                */
static int Width;                       /* width of an object in bytes                  */
static char *Base;                      /* pointer to element [-1] of array             */


static void newsift_down(L,U) int L,U;
{  int c;

   while(1)
      {c=L+L;
      if(c>U) break;
      if( (c+Width <= U) && ((*Comp)(Base+c+Width,Base+c)>0) ) c+= Width;
      if ((*Comp)(Base+L,Base+c)>=0) break;
      (*Switch)(Base+L, Base+c);
      L=c;
      }
}

void hsort(char * base, int nel, int width, int (*compare)(), void (*switcher)())
{
static int i,n,stop;
        /*      Perform a heap sort on an array starting at base.  The array is
                nel elements large and width is the size of a single element in
                bytes.  Compare is a pointer to a comparison routine which will
                be passed pointers to two elements of the array.  It should
                return a negative number if the left-most argument is less than
                the rightmost, 0 if the two arguments are equal, a positive
                number if the left argument is greater than the right.  (That
                is, it acts like a "subtract" operator.) If compare is 0 then
                the default comparison routine, argvcmp (which sorts an
                argv-like array of pointers to strings), is used.                                       */

   Width=width;
   Comp= compare;
   Switch= switcher;
   n=nel*Width;
   Base=base-Width;
   for (i=(n/Width/2)*Width; i>=Width; i-=Width) newsift_down(i,n);
   stop=Width+Width;
   for (i=n; i>=stop; )
      {
      (*Switch)(base, Base+i);
      newsift_down(Width,i-=Width);
      }

}

/*---------------------------------------------------------------------------*/

//******************************************************************************
//
// UL_GetPath
//
// Purpose
//    To parse the directory entered by the user to make the directory.
//
// Parms
//    Path - the path to be parsed.
//
// Returns
//    Pointer to next path
//
//******************************************************************************

char * UL_GetPath (char * path, char *dir)
{
   boolean done      = 0;
   char *dr          = dir;
   int cnt           = 0;

   if (*path == SLASHES)
      path++;

   while (!done)
   {
      *dr = *path;

      cnt++;                  // make sure the number of characters in the dir
      if (cnt > MAXCHARS)     // name doesn't exceed acceptable limits.
         Error ("ERROR : Directory name can only be %d characters long.\n", MAXCHARS);

      path++;
      dr++;

      if ((*path == SLASHES) || (*path == 0))
         done = true;
   }

   *dr = 0;
   return (path);
}


//******************************************************************************
//
// UL_ChangeDirectory ()
//
// Purpose
//    To change to a directory.  Checks for drive changes.
//
// Parms
//    path - The path to change to.
//
// Returns
//    TRUE  - If successful.
//    FALSE - If unsuccessful.
//
//******************************************************************************

boolean UL_ChangeDirectory (char *path)
{
	if (!path || !*path) {
		return true;
	}
	
	if (chdir (path) == -1) {
	         return (false);
	}
	
	return true;
}



//******************************************************************************
//
// UL_ChangeDrive ()
//
// Purpose
//    To change drives.
//
// Parms
//    drive - The drive to change to.
//
// Returns
//    TRUE  - If drive change successful.
//    FALSE - If drive change unsuccessful.
//
//******************************************************************************

boolean UL_ChangeDrive (char *drive)
{
	STUB_FUNCTION;
	
	return false;
}


/*
=============
=
= AbortCheck
=
=============
*/
void AbortCheck (char * abortstring)
{
   // User abort check

   IN_UpdateKeyboard ();

   if (Keyboard[sc_Escape])
      Error("%s\n",abortstring);
}

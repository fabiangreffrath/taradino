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
//    RT_UTIL.C - various utils palette funcs and modex stuff
//
//***************************************************************************

#ifndef _rt_util_public
#define _rt_util_public

#include <stdbool.h>
#include <stdint.h>

#define ERRORVERSIONROW 1
#define ERRORVERSIONCOL 67

#include "rt_def.h"
#include "m_misc2.h"
#include "develop.h"

extern  int    egacolor[16];
extern  uint8_t   *  origpal;
extern  int      _argc;
extern  char **  _argv;

void  markgetch( void );
bool StringsNotEqual (char * s1, char * s2, int length);
void  GetPalette(char * pal);
void  ClearGraphicsScreen( void );
void  ClearBuffer( char * buf, int size );
void  Error (char *error, ...) __attribute__((noreturn,format(printf,1,2)));
void  StartupSoftError ( void );
void  ShutdownSoftError ( void );
int   CheckParm (char *check);
int   SafeOpenWrite (char *filename);
int   SafeOpenAppend (char *filename);
int   SafeOpenRead (char *filename);
void  SafeRead (int handle, void *buffer, long count);
void  SafeWrite (int handle, void *buffer, long count);
void  SafeWriteString (int handle, char * buffer);
void  *SafeMalloc (long size);
void  *SafeLevelMalloc (long size);
void  SafeFree (void * ptr);
long  LoadFile (char *filename, void **bufferptr);
void  SaveFile (char *filename, void *buffer, long count);
void  DefaultExtension (char *path, char *extension);
void  DefaultPath (char *path, char *basepath);
void  ExtractFileBase (char *path, char *dest);
long  ParseHex (char *hex);
long  ParseNum (char *str);
short MotoShort (short l);
short IntelShort (short l);
int   MotoLong (int l);
int   IntelLong (int l);
void  SwapIntelLong (int *l);
void  SwapIntelShort(short *s);
void  SwapIntelLongArray (int *l, int num);
void  SwapIntelShortArray (short *s, int num);
int   US_CheckParm (char *parm, char **strings);
uint8_t  BestColor (int r, int g, int b, uint8_t *palette);
int   atan2_appx(int,int);
int   FindDistance(int ix, int iy);
int   Find_3D_Distance(int ix, int iy, int iz);
void  SetPalette ( uint8_t * pal );
void  SetaPalette ( uint8_t * pal );
void  FindEGAColors ( void );
void  VL_FillPalette (int red, int green, int blue);
void  VL_SetColor  (int color, int red, int green, int blue);
void  VL_GetColor  (int color, int *red, int *green, int *blue);
void  VL_SetPalette (uint8_t *palette);
void  VL_GetPalette (uint8_t *palette);
void  UL_printf (char *str);
void  VL_NormalizePalette (uint8_t *palette);
void  MapDebug (char *error, ...) __attribute__((format(printf,1,2)));
void  OpenMapDebug ( void );
void  UL_ColorBox (int x, int y, int w, int h, int color);

void UL_DisplayMemoryError ( int memneeded );

int   SideOfLine(int x1, int y1, int x2, int y2, int x3, int y3);

void hsort(char * base, int nel, int width, int (*compare)(), void (*switcher)());

char * UL_GetPath (char * path, char *dir);
bool UL_ChangeDirectory (char *path);
bool UL_ChangeDrive (char *drive);
void AbortCheck (char * abortstring);

struct dosdate_t
{
    uint8_t day;
    uint8_t month;
    uint32_t year;
    uint8_t dayofweek;
};

void _dos_getdate(struct dosdate_t *date);

void *safe_realloc (void *ptr, size_t size);

#if (SOFTERROR==1)

void  SoftwareError (char *error, ...) __attribute__((format(printf,1,2)));
#define SoftError  SoftwareError

#else
void  SoftwareError (char *error, ...) __attribute__((format(printf,1,2)));
//#define SoftError  SoftwareError

#define SoftError  if (1) {} else SoftwareError

//#define SoftError

#endif

void  DebugError (char *error, ...) __attribute__((format(printf,1,2)));
#define Debug  DebugError
//#define Debug

void Square (void);

#define my_outp(a,b)

#define OUTP                              my_outp

#endif

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
#ifndef _lumpy_
#define _lumpy_

#include <stdint.h>

//****************************************************************************
//
// Public header for LUMPY typedefs
//
//****************************************************************************


typedef struct
{
   uint8_t     width,height;
   uint8_t     data;
} pic_t;

#define CONVERT_ENDIAN_pic_t(pp) { }

typedef struct
{
   int16_t     width,height;
   int16_t     orgx,orgy;
   uint8_t     data;
} lpic_t;

#define CONVERT_ENDIAN_lpic_t(lp)            \
    {                                        \
        SwapIntelShort(&lp->width);          \
        SwapIntelShort(&lp->height);         \
        SwapIntelShort(&lp->orgx);           \
        SwapIntelShort(&lp->orgy);           \
    }
    
typedef struct
{
	int16_t height;
   char  width[256];
   int16_t charofs[256];
   uint8_t  data;       // as much as required
} font_t;

#define CONVERT_ENDIAN_font_t(fp)            \
    {                                        \
        int i;                               \
        SwapIntelShort(&fp->height);         \
        for (i = 0; i < 256; i++) {          \
            SwapIntelShort(&fp->charofs[i]); \
        }                                    \
    }

typedef struct
{
   int16_t width;
   int16_t height;
   uint8_t palette[768];
   uint8_t data;
} lbm_t;

#define CONVERT_ENDIAN_lbm_t(lp)             \
    {                                        \
        SwapIntelShort(&lp->width);          \
        SwapIntelShort(&lp->height);         \
    }
 
typedef struct
{
   int16_t          origsize;         // the orig size of "grabbed" gfx
   int16_t          width;            // bounding box size
   int16_t          height;
   int16_t          leftoffset;       // pixels to the left of origin
   int16_t          topoffset;        // pixels above the origin
   uint16_t collumnofs[320];  // only [width] used, the [0] is &collumnofs[width]
} patch_t;

#define CONVERT_ENDIAN_patch_t(pp)           \
    {                                        \
        int i;                               \
        SwapIntelShort(&pp->origsize);       \
        SwapIntelShort(&pp->width);          \
        SwapIntelShort(&pp->height);         \
        SwapIntelShort(&pp->leftoffset);     \
        SwapIntelShort(&pp->topoffset);      \
        for (i = 0; i < pp->width; i++) {          \
            SwapIntelShort((int16_t*)&pp->collumnofs[i]); \
        }                                    \
    }

typedef struct
{
   int16_t origsize;         // the orig size of "grabbed" gfx
   int16_t width;            // bounding box size
   int16_t height;
   int16_t leftoffset;       // pixels to the left of origin
   int16_t topoffset;        // pixels above the origin
   int16_t translevel;
   int16_t collumnofs[320];  // only [width] used, the [0] is &collumnofs[width]
} transpatch_t;

#define CONVERT_ENDIAN_transpatch_t(pp)      \
    {                                        \
        int i;                               \
        SwapIntelShort(&pp->origsize);       \
        SwapIntelShort(&pp->width);          \
        SwapIntelShort(&pp->height);         \
        SwapIntelShort(&pp->leftoffset);     \
        SwapIntelShort(&pp->topoffset);      \
        SwapIntelShort(&pp->translevel);     \
        for (i = 0; i < pp->width; i++) {          \
            SwapIntelShort((int16_t*)&pp->collumnofs[i]); \
        }                                    \
    }

typedef struct
{
   uint8_t  color;
   int16_t height;
   char  width[256];
   int16_t charofs[256];
   uint8_t  pal[0x300];
   uint8_t  data;       // as much as required
} cfont_t;

#define CONVERT_ENDIAN_cfont_t(pp)           \
    {                                        \
        int i;                               \
        SwapIntelShort(&pp->height);         \
        for (i = 0; i < 256; i++) {          \
            SwapIntelShort(&pp->charofs[i]); \
        }                                    \
    }

#endif

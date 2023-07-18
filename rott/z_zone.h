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
//    Z_Zone Memory management Constants
//
//***************************************************************************

#ifndef _z_zone_public
#define _z_zone_public

#include <sys/types.h>

extern int lowmemory;

typedef enum {
    // tags < PU_PURGELEVEL are not overwritten until freed
    PU_STATIC,      // static entire execution time
    PU_LEVEL,       // start of static until level exited
    // tags >= PU_PURGELEVEL are purgable whenever needed
    PU_CACHE,
    PU_MAX
} pu_tag;

#define PU_GAME PU_STATIC

#define PU_LEVELSTRUCT PU_LEVEL
#define PU_LEVELEND PU_LEVEL
#define URGENTLEVELSTART PU_LEVEL

#define PU_PURGELEVEL PU_CACHE
#define PU_FLAT PU_CACHE
#define PU_PATCH PU_CACHE
#define PU_TEXTURE PU_CACHE
#define PU_CACHESOUNDS PU_CACHE
#define PU_CACHEACTORS PU_CACHE
#define PU_CACHEBJWEAP PU_CACHE
#define PU_CACHESPRITES PU_CACHE
#define PU_CACHEWALLS PU_CACHE

//***************************************************************************
//
//    Z_ZONE.C - Carmack's Memory manager for protected mode
//
//***************************************************************************

extern int zonememorystarted;

void Z_Init (int size, int min);                // Starts up Memory manager (size is in bytes), (min is minimum requirement)
void Z_Free (void *ptr);                        // Free a pointer in Z_Zone's domain
void *Z_Malloc (size_t size, pu_tag tag, void **user); // Malloc You can pass a NULL user if the tag is < PU_PURGELEVEL
void Z_FreeTags (pu_tag lowtag, pu_tag hightag);      // Free a series of memory tags
void Z_ChangeTag (void *ptr, pu_tag tag);          // Change the tag of a memory item
size_t Z_HeapSize ( void );                        // Return the total heap size
size_t Z_UsedHeap ( void );                        // Return used portion of heap size
void Z_ShutDown( void );
void Z_Realloc (void **ptr, size_t newsize);

#define Z_LevelMalloc(a,b,c) Z_Malloc(a,b,c)
#define Z_UsedLevelHeap() Z_UsedHeap()

#endif

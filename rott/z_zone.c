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
#include "rt_util.h"
#include "z_zone.h"

#define MAXMEMORYSIZE (8 << 20)

int zonememorystarted = 0;

// Minimum chunk size at which blocks are allocated
#define CHUNK_SIZE sizeof(void *)

// signature for block header
#define ZONEID (0x1a2b3c4d)

typedef struct memblock
{
	struct memblock *next, *prev;
	size_t size;
	void **user;
	unsigned int id;
	pu_tag tag;
} memblock_t;

static const size_t HEADER_SIZE =
	(sizeof(memblock_t) + CHUNK_SIZE - 1) & ~(CHUNK_SIZE - 1);

static memblock_t *blockbytag[PU_MAX];
static size_t heapsize;

void Z_Init(int size, int min)
{
	zonememorystarted = 1;
	heapsize = 0;
}

void Z_ShutDown(void)
{
	zonememorystarted = 0;
}

// Z_Malloc
// You can pass a NULL user if the tag is < PU_CACHE.

static void Z_FreeTag(pu_tag tag);

void *Z_Malloc(size_t size, pu_tag tag, void **user)
{
	memblock_t *block = NULL;

	if (tag == PU_CACHE && !user)
		Error("Z_Malloc: An owner is required for purgable blocks");

	if (!size)
		return user ? *user = NULL : NULL; // malloc(0) returns NULL

	while (!(block = malloc(size + HEADER_SIZE)))
	{
		if (!blockbytag[PU_CACHE])
			Error("Z_Malloc: Failure trying to allocate %lu bytes",
				  (unsigned long)size);
		Z_FreeTag(PU_CACHE);
	}

	if (!blockbytag[tag])
	{
		blockbytag[tag] = block;
		block->next = block->prev = block;
	}
	else
	{
		blockbytag[tag]->prev->next = block;
		block->prev = blockbytag[tag]->prev;
		block->next = blockbytag[tag];
		blockbytag[tag]->prev = block;
	}

	block->size = size;
	block->id = ZONEID; // signature required in block header
	block->tag = tag;	// tag
	block->user = user; // user
	block = (memblock_t *)((char *)block + HEADER_SIZE);
	if (user)		   // if there is a user
		*user = block; // set user to point to new block

	heapsize += size;

	return block;
}

void Z_Free(void *p)
{
	memblock_t *block = (memblock_t *)((char *)p - HEADER_SIZE);

	if (!p)
		return;

	if (block->id != ZONEID)
		Error("Z_Free: freed a pointer without ZONEID");
	block->id = 0; // Nullify id so another free fails

	if (block->user) // Nullify user if one exists
		*block->user = NULL;

	if (block == block->next)
		blockbytag[block->tag] = NULL;
	else if (blockbytag[block->tag] == block)
		blockbytag[block->tag] = block->next;
	block->prev->next = block->next;
	block->next->prev = block->prev;

	heapsize -= block->size;

	free(block);
}

static void Z_FreeTag(pu_tag tag)
{
	memblock_t *block, *end_block;

	if (tag < 0 || tag >= PU_MAX)
		Error("Z_FreeTag: Tag %i does not exist", tag);

	block = blockbytag[tag];
	if (!block)
		return;
	end_block = block->prev;
	while (1)
	{
		memblock_t *next = block->next;
		Z_Free((char *)block + HEADER_SIZE);
		if (block == end_block)
			break;
		block = next; // Advance to next block
	}
}

void Z_FreeTags(pu_tag lowtag, pu_tag hightag)
{
	pu_tag tag;

	for (tag = lowtag; tag <= hightag; tag++)
	{
		Z_FreeTag(tag);
	}
}

void Z_ChangeTag(void *ptr, pu_tag tag)
{
	memblock_t *block = (memblock_t *)((char *)ptr - HEADER_SIZE);

	// proff - added sanity check, this can happen when an empty lump is locked
	if (!ptr)
		return;

	// proff - do nothing if tag doesn't differ
	if (tag == block->tag)
		return;

	if (block->id != ZONEID)
		Error("Z_ChangeTag: freed a pointer without ZONEID");

	if (tag == PU_CACHE && !block->user)
		Error("Z_ChangeTag: an owner is required for purgable blocks\n");

	if (block == block->next)
		blockbytag[block->tag] = NULL;
	else if (blockbytag[block->tag] == block)
		blockbytag[block->tag] = block->next;
	block->prev->next = block->next;
	block->next->prev = block->prev;

	if (!blockbytag[tag])
	{
		blockbytag[tag] = block;
		block->next = block->prev = block;
	}
	else
	{
		blockbytag[tag]->prev->next = block;
		block->prev = blockbytag[tag]->prev;
		block->next = blockbytag[tag];
		blockbytag[tag]->prev = block;
	}

	block->tag = tag;
}

size_t Z_HeapSize(void)
{
	return (size_t)MAXMEMORYSIZE;
}

size_t Z_UsedHeap(void)
{
	return heapsize;
}

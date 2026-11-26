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

// Both sound tables define the same enum constants, so each one is included
// in the scope of its own function. This file is meant to be included once
// per translation unit, after _rt_soun.h and rt_sound.h.

static sound_t *RegisteredSounds(void)
{
#include "snd_reg.h"
	return sounds;
}

static sound_t *SharewareSounds(void)
{
#include "snd_shar.h"
	return sounds;
}

static sound_t *SoundsTable(void)
{
	static sound_t *table;

	if (table == NULL)
		table = rott_shareware ? SharewareSounds() : RegisteredSounds();

	return table;
}

#define sounds SoundsTable()

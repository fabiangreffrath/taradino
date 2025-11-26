/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2025 Fabian Greffrath

 This package is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 .
 This package is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 .
 You should have received a copy of the GNU General Public License
 along with this package. If not, see <https://www.gnu.org/licenses/>.
*/

#include "music.h"

int Dummy_II(int dummy)
{
	(void)dummy;

	return MUSIC_Ok;
}

static int Dummy_IV(void)
{
	return __FX_FALSE;
}

static void Dummy_VV(void)
{
}

static int Dummy_ICII(unsigned char *song, int size, int loopflag)
{
	(void)song;
	(void)size;
	(void)loopflag;

	return MUSIC_Ok;
}

static int Dummy_III(int tovolume, int milliseconds)
{
	(void)tovolume;
	(void)milliseconds;

	return MUSIC_Ok;
}

music_module_t dummy_music_module = {
	Dummy_II, Dummy_IV,	  Dummy_IV,	 Dummy_VV, Dummy_VV,
	Dummy_IV, Dummy_ICII, Dummy_III, Dummy_IV,
};

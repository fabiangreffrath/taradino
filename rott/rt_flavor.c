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

//******************************************************************************
//
// RT_FLAVOR.C
//    ROTT flavor selection
//
//******************************************************************************

#include "rt_flavor.h"

/* standard shareware */
rt_flavor_t flavor_huntbgin = {
	.name = "The HUNT Begins",
	.num_rtls = 1,
	.rtls[0] = "huntbgin.rtl",
	.num_rtcs = 1,
	.rtcs[0] = "huntbgin.rtc",
	.num_wads = 2,
	.wads[0] = "huntbgin.wad",
	.wads[1] = "remote1.rts"
};

/* deluxe shareware */
rt_flavor_t flavor_huntbgn2 = {
	.name = "The HUNT Begins (Deluxe)",
	.num_rtls = 1,
	.rtls[0] = "huntbgn2.rtl",
	.num_rtcs = 1,
	.rtcs[0] = "huntbgn2.rtc",
	.num_wads = 2,
	.wads[0] = "huntbgin.wad",
	.wads[1] = "remote1.rts"
};

/* base game */
rt_flavor_t flavor_darkwar = {
	.name = "Dark War",
	.num_rtls = 1,
	.rtls[0] = "darkwar.rtl",
	.num_rtcs = 1,
	.rtcs[0] = "darkwar.rtc",
	.num_wads = 2,
	.wads[0] = "darkwar.wad",
	.wads[1] = "remote1.rts"
};

/* extreme rott */
rt_flavor_t flavor_extreme = {
	.name = "Extreme ROTT",
	.num_rtls = 1,
	.rtls[0] = "extreme.rtl",
	.num_rtcs = 0,
	.num_wads = 2,
	.wads[0] = "darkwar.wad",
	.wads[1] = "remote1.rts"
};

/* new blood campaign */
rt_flavor_t flavor_huntcont = {
	.name = "The HUNT Continues",
	.num_rtls = 1,
	.rtls[0] = "huntcontEX.rtlx",
	.num_wads = 2,
	.wads[0] = "darkwar.wad",
	.wads[1] = "remote1.rts"
};

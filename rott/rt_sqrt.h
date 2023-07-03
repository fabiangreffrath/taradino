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
#ifndef _rt_sqrt_public
#define _rt_sqrt_public

/*
FUNCTION:
    Fixed32 FixedSqrtHP(Fixed32 n);
DESCRIPTION:
    This does a high-precision square root of a Fixed32.  It has
    8.16 bit accuracy.  For more speed use FixedSqrtLP().


FUNCTION:
    Fixed32 FixedSqrtLP(Fixed32 n);
DESCRIPTION:
    This does a low-precision square root of a Fixed32.  It has
    8.8 bit accuracy.  For more accuracy use FixedSqrtHP().
*/



long FixedSqrtLP(long n);  // Low  Precision (8.8)
long FixedSqrtHP(long n);  // High Precision (8.16)

#endif

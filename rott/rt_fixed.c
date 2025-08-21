#include "rt_def.h"

#include "rt_fixed.h"

/*
  C versions of watcom.h assembly.
 */

fixed FixedMul(fixed a, fixed b)
{
	int64_t scratch1 = (int64_t)a * (int64_t)b + (int64_t)0x8000;
	return (scratch1 >> 16) & 0xffffffff;
}

fixed FixedMulShift(fixed a, fixed b, fixed shift)
{
	int64_t x = a;
	int64_t y = b;
	int64_t z = x * y;

	return (((uint64_t)z) >> shift) & 0xffffffff;
}

fixed FixedDiv2(fixed a, fixed b)
{
	int64_t x = (signed int)a;
	int64_t y = (signed int)b;
	int64_t z = x * 65536 / y;

	return (z) & 0xffffffff;
}

fixed FixedScale(fixed orig, fixed factor, fixed divisor)
{
	int64_t x = orig;
	int64_t y = factor;
	int64_t z = divisor;

	int64_t w = (x * y) / z;

	return (w) & 0xffffffff;
}

/*
 *  C version of fixed-point Square Root function
 */

fixed FixedSqrt(fixed n) // High Precision (8.16)
{
	/* This is more or less a direct C transliteration of the asm code.
	 *	   I've replaced right shifting with division, since right shifting
	 *	   signed values is undefined in ANSI C (though it usually works).
	 *	   ROTT does not use this routine heavily. */
	ufixed root, mask, val;
	fixed d;

	root = 0;
	mask = 0x40000000;
	val = (ufixed)n;
hp1:
	d = val;
	d -= mask;
	if (d < 0)
		goto hp2;
	d -= root;
	if (d < 0)
		goto hp2;

	val = d;
	root /= 2;
	root |= mask;
	mask /= 4;
	if (mask != 0)
		goto hp1;
	else
		goto hp5;
hp2:
	root /= 2;
	mask /= 4;
	if (mask != 0)
		goto hp1;

hp5:
	mask = 0x00004000;
	root <<= 16;
	val <<= 16;
hp3:
	/* use add here to properly emulate the asm - SBF */
	if ((root + mask) > val)
		goto hp4;

	val -= (root + mask);

	root /= 2;
	root |= mask;
	mask /= 4;
	if (mask != 0)
		goto hp3;
	else
		goto hp6;
hp4:
	root /= 2;
	mask /= 4;
	if (mask != 0)
		goto hp3;
hp6:

	return (fixed)root;
}

#include "rt_def.h"

#include "watcom.h"

/* 
  C versions of watcom.h assembly.
 */

fixed FixedMul(fixed a, fixed b)
{
	int64_t scratch1 = (int64_t) a * (int64_t) b + (int64_t) 0x8000;
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

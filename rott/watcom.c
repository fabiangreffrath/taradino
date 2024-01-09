#include "rt_def.h"

#include "watcom.h"

/* 
  C versions of watcom.h assembly.
 */

int32_t FixedMul(int32_t a, int32_t b)
{
	int64_t scratch1 = (int64_t) a * (int64_t) b + (int64_t) 0x8000;
	return (scratch1 >> 16) & 0xffffffff;
}

int32_t FixedMulShift(int32_t a, int32_t b, int32_t shift)
{
	int64_t x = a;
	int64_t y = b;
	int64_t z = x * y;
	
	return (((uint64_t)z) >> shift) & 0xffffffff;
}

int32_t FixedDiv2(int32_t a, int32_t b)
{
	int64_t x = (signed int)a;
	int64_t y = (signed int)b;
	int64_t z = x * 65536 / y;
	
	return (z) & 0xffffffff;
}

int32_t FixedScale(int32_t orig, int32_t factor, int32_t divisor)
{
	int64_t x = orig;
	int64_t y = factor;
	int64_t z = divisor;
	
	int64_t w = (x * y) / z;
	
	return (w) & 0xffffffff;
}

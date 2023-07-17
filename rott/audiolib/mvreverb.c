#include "multivoc.h"
#include "_multivc.h"

void MV_16BitReverb( const char *src, char *dest, const VOLUME16 *volume, int count )
{
	int i;

	short *pdest = (short *)dest;
	
	for (i = 0; i < count; i++) {
		int sl = src[i*2+0];
		int sh = src[i*2+1] ^ 0x80;
		
		sl = (*volume)[sl] >> 8;
		sh = (*volume)[sh];
		
		pdest[i] = (short)(sl + sh + 0x80);
	}
}

void MV_8BitReverb( const char *src, char *dest, const VOLUME16 *volume, int count )
{
	int i;

	for (i = 0; i < count; i++) {
		unsigned char s = (unsigned char) src[i];
		
		s = (*volume)[s] & 0xff;
		
		dest[i] = (char)(s + 0x80);
	}
}

void MV_16BitReverbFast( const char *src, char *dest, int count, int shift )
{
	int i;

	short *pdest = (short *)dest;
	const short *psrc = (const short *)src;
	
	for (i = 0; i < count; i++) {
		pdest[i] = psrc[i] >> shift;
	}
}

void MV_8BitReverbFast( const char *src, char *dest, int count, int shift )
{
	int i;

	unsigned char sh = 0x80 - (0x80 >> shift);
	
	for (i = 0; i < count; i++) {
		unsigned char a = ((unsigned char) src[i]) >> shift;
		unsigned char c = (((unsigned char) src[i]) ^ 0x80) >> 7;
		
		dest[i] = (signed char) (a + sh + c);
	}
}

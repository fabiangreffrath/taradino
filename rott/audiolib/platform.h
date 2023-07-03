#ifndef _INCLUDE_PLATFORM_H_
#define _INCLUDE_PLATFORM_H_

static __inline unsigned short _swap16(unsigned short D)
{
    return((D<<8)|(D>>8));
}

static __inline unsigned int _swap32(unsigned int D)
{
    return((D<<24)|((D<<8)&0x00FF0000)|((D>>8)&0x0000FF00)|(D>>24));
}

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define PLATFORM_BIGENDIAN 1
#define BUILDSWAP_INTEL16(x) _swap16(x)
#define BUILDSWAP_INTEL32(x) _swap32(x)
#else
#define PLATFORM_LITTLEENDIAN 1
#define BUILDSWAP_INTEL16(x) (x)
#define BUILDSWAP_INTEL32(x) (x)
#endif

#endif  /* !defined _INCLUDE_PLATFORM_H_ */

/* end of platform.h ... */



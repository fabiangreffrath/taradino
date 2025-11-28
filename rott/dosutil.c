#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#ifndef _WIN32
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#endif

#include <fcntl.h>

#include "rt_def.h"

#include "SDL.h"

/*
  Copied over from Wolf3D Linux: http://www.icculus.org/wolf3d/
  Modified for ROTT.
 */

int _argc;
char **_argv;

#ifndef _MSC_VER
long filelength(int handle)
{
	struct stat buf;

	if (fstat(handle, &buf) == -1)
	{
		perror("filelength");
		exit(EXIT_FAILURE);
	}

	return buf.st_size;
}

char *strupr(char *s)
{
	char *p = s;

	while (*p)
	{
		*p = toupper(*p);
		p++;
	}

	return s;
}

char *itoa(int value, char *string, int radix)
{
	switch (radix)
	{
		case 10:
			sprintf(string, "%d", value);
			break;
		case 16:
			sprintf(string, "%x", value);
			break;
		default:
			fprintf(stderr, "itoa: unsupported radix %d", radix);
			break;
	}

	return string;
}

char *ltoa(long value, char *string, int radix)
{
	switch (radix)
	{
		case 10:
			sprintf(string, "%ld", value);
			break;
		case 16:
			sprintf(string, "%lx", value);
			break;
		default:
			fprintf(stderr, "ltoa: unsupported radix %d", radix);
			break;
	}

	return string;
}

char *ultoa(unsigned long value, char *string, int radix)
{
	switch (radix)
	{
		case 10:
			sprintf(string, "%lu", value);
			break;
		case 16:
			sprintf(string, "%lux", value);
			break;
		default:
			fprintf(stderr, "ultoa: unsupported radix %d", radix);
			break;
	}

	return string;
}
#endif

char getch(void)
{
	getchar();
	return 0;
}

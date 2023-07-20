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

long filelength(int handle)
{
	struct stat buf;
	
	if (fstat(handle, &buf) == -1) {
		perror("filelength");
		exit(EXIT_FAILURE);
	}
	
	return buf.st_size;
}

char *strlwr(char *s)
{
	char *p = s;
	
	while (*p) {
		*p = tolower(*p);
		p++;
	}
	
	return s;
}

char *strupr(char *s)
{
	char *p = s;
	
	while (*p) {
		*p = toupper(*p);
		p++;
	}
	
	return s;
}
	
char *itoa(int value, char *string, int radix)
{
	switch (radix) {
		case 10:
			sprintf(string, "%d", value);
			break;
		case 16:
			sprintf(string, "%x", value);
			break;
		default:
			STUB_FUNCTION;
			break;
	}
	
	return string;
}

char *ltoa(long value, char *string, int radix)
{
	switch (radix) {
		case 10:
			sprintf(string, "%ld", value);
			break;
		case 16:
			sprintf(string, "%lx", value);
			break;
		default:
			STUB_FUNCTION;
			break;
	}
	
	return string;
}

char *ultoa(unsigned long value, char *string, int radix)
{
	switch (radix) {
		case 10:
			sprintf(string, "%lu", value);
			break;
		case 16:
			sprintf(string, "%lux", value);
			break;
		default:
			STUB_FUNCTION;
			break;
	}
	
	return string;
}

char getch(void)
{
	getchar();
	return 0;
}

/* from Dan Olson */
void put_dos2ansi(byte attrib)
{
	int lookup[] = {30,34,32,36,31,35,33,37};
	byte fore,back,blink=0,intens=0;
	
	fore = attrib&15;	/* bits 0-3 */
	back = attrib&112; /* bits 4-6 */
       	blink = attrib&128; /* bit 7 */
	
	/* Fix background, blink is either on or off. */
	back = back>>4;
 
	/* Fix foreground */
	if (fore > 7) {
		intens = 1;
		fore-=8;
	}

	/* Convert fore/back */
	fore = lookup[fore];
	back = lookup[back]+10;

	// 'Render"
	if (blink)
		printf ("\033[%d;5;%dm\033[%dm", intens, fore, back);
	else
		printf ("\033[%d;25;%dm\033[%dm", intens, fore, back);
}

void DisplayTextSplash(byte *text, int l)
{
	int i;
	int bound = 80*l*2;

	for (i=0;i<bound;i+=2)
	{
		put_dos2ansi(text[i+1]);
		putchar (text[i]);
	}

	printf ("\033[m");
}

#if !defined(__CYGWIN__) && !defined(__MINGW32__)
#include <execinfo.h>

void print_stack (int level)
{
	void *array[64];
	char **syms;
	int size, i;

	printf ("Stack dump:\n");
	printf ("{\n");
	size = backtrace (array, (sizeof (array))/(sizeof (array[0])));
	syms = backtrace_symbols (array, size);
	for (i=level+1; i<size;++i) {
		printf ("\t%s\n",syms[i]);
	}
	free (syms);
	/*
	for (i = 2; i <size; ++i) {
		printf ("\t%p\n", array[i]);
	}
	*/
	printf ("}\n");
}
#else

void print_stack (int level)
{
        printf("Stack dump not implemented.\n");
}

#endif

void crash_print (int sig)
{
	printf ("OH NO OH NO ROTT CRASHED!\n");
	printf ("Here is where:\n");
	print_stack (1);
	SDL_Quit ();
	exit (1);
}


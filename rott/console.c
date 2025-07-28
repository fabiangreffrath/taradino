/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2002-2015 Steven Fuller, Ryan C. Gordon, John Hall, Dan Olson
Copyright (C) 2006-2025 Fabian Greffrath
Copyright (C) 2023-2025 erysdren (it/its)

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

//****************************************************************************
//
// CONSOLE.C
//
//****************************************************************************

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "SDL.h"
#include "rt_def.h"
#include "rt_main.h"
#include "rt_str.h"
#include "rt_util.h"
#include "rt_build.h"
#include "rt_menu.h"
#include "rt_game.h"
#include "rt_ted.h"
#include "rt_datadir.h"
#include "states.h"
#include "sprites.h"
#include "rt_stat.h"
#include "rt_debug.h"
#include "w_wad.h"
#include "console.h"

extern char * ROTTMAPS;

//****************************************************************************
//
// CVAR
//
//****************************************************************************

/* global cvar chain */
static cvar_t *cvar_list = NULL;

/* retrieve cvar from chain */
cvar_t *cvar_retrieve(const char *name)
{
	cvar_t *cvar = cvar_list;

	while (cvar != NULL)
	{
		if (strcasecmp(name, cvar->name) == 0)
			return cvar;

		/* move down chain */
		cvar = cvar->next;
	}

	return NULL;
}

/* add cvar to chain */
void cvar_register(cvar_t *cvar)
{
	/* don't add it if there's already one in the chain with the same name */
	if (cvar_retrieve(cvar->name) != NULL)
		return;

	if (cvar_list == NULL)
	{
		/* start chain */
		cvar_list = cvar;
	}
	else
	{
		/* add to chain */
		cvar->next = cvar_list;
		cvar_list = cvar;
	}
}

boolean cvar_is_set(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return false;

	return cvar->flags & CVAR_FLAG_SET;
}

boolean cvar_is_protected(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return false;

	return cvar->flags & CVAR_FLAG_PROTECTED;
}

boolean cvar_get_bool(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return false;
	if (cvar->type != CVAR_TYPE_BOOL)
		return false;

	return cvar->val.b;
}

int cvar_get_int(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return 0;
	if (cvar->type != CVAR_TYPE_INT)
		return 0;

	return cvar->val.i;
}

unsigned int cvar_get_uint(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return 0;
	if (cvar->type != CVAR_TYPE_UINT)
		return 0;

	return cvar->val.u;
}

fixed cvar_get_fixed(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return 0;
	if (cvar->type != CVAR_TYPE_FIXED)
		return 0;

	return cvar->val.x;
}

float cvar_get_float(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return 0;
	if (cvar->type != CVAR_TYPE_FLOAT)
		return 0;

	return cvar->val.f;
}

const char *cvar_get_string(const char *name)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return NULL;
	if (cvar->type != CVAR_TYPE_STRING)
		return NULL;

	return cvar->val.s;
}

/* set cvar */
boolean cvar_set(const char *name, const char *value)
{
	cvar_t *cvar;

	if ((cvar = cvar_retrieve(name)) == NULL)
		return false;

	/* set value */
	switch (cvar->type)
	{
		case CVAR_TYPE_BOOL:
			if (strcasecmp(value, "true") == 0)
				cvar->val.b = true;
			else if (strcasecmp(value, "false") == 0)
				cvar->val.b = false;
			else if (strtol(value, NULL, 10))
				cvar->val.b = true;
			else if (!strtol(value, NULL, 10))
				cvar->val.b = false;
			else
				cvar->val.b = false;
			break;

		case CVAR_TYPE_INT:
			cvar->val.i = strtol(value, NULL, 10);
			break;

		case CVAR_TYPE_UINT:
			cvar->val.u = strtoul(value, NULL, 10);
			break;

		case CVAR_TYPE_FIXED:
			cvar->val.x = FIXED(strtof(value, NULL));
			break;

		case CVAR_TYPE_FLOAT:
			cvar->val.f = strtof(value, NULL);
			break;

		case CVAR_TYPE_STRING:
			if (cvar->flags & CVAR_FLAG_SET)
				free(cvar->val.s);
			cvar->val.s = M_StringDuplicate(value);
			break;

		default:
			Error("cvar %s with type %d", cvar->name, cvar->type);
			break;
	}

	/* add set flag */
	cvar->flags |= CVAR_FLAG_SET;

	return true;
}

//****************************************************************************
//
// CVARLIB
//
//****************************************************************************

/* cvarlib array */
cvar_t _cvarlib[] = {
	CVAR_STRING("fs_root", "./", CVAR_FLAG_PROTECTED),
	CVAR_STRING("fs_game", "darkwar", CVAR_FLAG_PROTECTED)
};

/* register standard library of cvars */
void cvarlib_init(void)
{
	for (int i = 0; i < sizeof(_cvarlib) / sizeof(cvar_t); i++)
	{
		cvar_register(&_cvarlib[i]);
	}
}

/* shutdown cvarlib */
void cvarlib_quit(void)
{
	cvar_t *cvar = cvar_list;

	while (cvar != NULL)
	{
		/* free value string if it's been set */
		if (cvar->type == CVAR_TYPE_STRING && cvar->flags & CVAR_FLAG_SET)
			free(cvar->val.s);

		/* next */
		cvar = cvar->next;
	}

	cvar_list = NULL;
}

//****************************************************************************
//
// CMD
//
//****************************************************************************

/* global cmd chain */
static cmd_t *cmd_list = NULL;

/* retrieve cmd from chain */
cmd_t *cmd_retrieve(const char *name)
{
	cmd_t *cmd = cmd_list;

	while (cmd != NULL)
	{
		if (strcasecmp(name, cmd->name) == 0)
			return cmd;

		/* move down chain */
		cmd = cmd->next;
	}

	return NULL;
}

/* add cmd to chain */
void cmd_register(cmd_t *cmd)
{
	/* don't add it if there's already one in the chain with the same name */
	if (cmd_retrieve(cmd->name) != NULL)
		return;

	if (cmd_list == NULL)
	{
		/* start chain */
		cmd_list = cmd;
	}
	else
	{
		/* add to chain */
		cmd->next = cmd_list;
		cmd_list = cmd;
	}
}

//****************************************************************************
//
// CMDLIB
//
//****************************************************************************

/* quit */
int _cmd_quit(int argc, char **argv)
{
	QuitGame();
	SDL_Quit();
	exit(0);
	return 0;
}

/* map */
int _cmd_map(int argc, char **argv)
{
	int episode = 0, level = 0, map = 0;
	int i;

	if (!ingame)
	{
		console_printf("can't change or query map while not in-game");
		return 0;
	}

	/* print current map */
	if (argc < 2)
	{
		console_printf("%s: E%dA%d - \"%s\" (%d)",
			ROTTMAPS,
			gamestate.episode,
			GetLevel(gamestate.episode, gamestate.mapon),
			LevelName,
			gamestate.mapon
		);
		return 0;
	}

	/* convert to lowercase */
	for (i = 0; i < strlen(argv[1]); i++)
		argv[1][i] = tolower(argv[1][i]);

	/* try to figure out map from string */
	sscanf(argv[1], "e%da%d", &episode, &level);

	/* bruteforce string to int */
	if (!episode || !level)
	{
		map = strtol(argv[1], NULL, 10);
		if (map >= 100 || map < 0)
		{
			console_printf("map %d is out of range", map);
			return 1;
		}
	}
	else
	{
		/* range checks */
		if (episode > 4 || (episode == 4 && level > 13) || (episode < 4 && level > 8))
		{
			console_printf("map E%dA%d is out of range", episode, level);
			return 1;
		}
		else
		{
			/* get map */
			map = ((episode - 1) * 8) + (level - 1);
		}
	}

	/* don't do it if we're already on this map */
	if (gamestate.mapon == map)
	{
		console_printf("map %d is already loaded", map);
		return 1;
	}

	/* force close console and do warp */
	console_should_close = true;
	playstate = ex_warped;
	gamestate.mapon = map;

	return 0;
}

/* mapset */
int _cmd_mapset(int argc, char **argv)
{
	char *filename;

	if (argc < 2)
	{
		console_printf("%s", ROTTMAPS);
		return 0;
	}

	if (ingame)
	{
		console_printf("cannot set new mapset while in-game");
		return 1;
	}

	filename = M_FileCaseExists(argv[1]);

	if (!filename)
	{
		console_printf("mapset \"%s\" not found", argv[1]);
		return 1;
	}

	ROTTMAPS = filename;

	console_printf("mapset changed to \"%s\"", ROTTMAPS);

	return 0;
}

/* maps */
int _cmd_maps(int argc, char **argv)
{
	mapfileinfo_t mapinfo;
	int i, level, map;

	GetMapInfo(&mapinfo);

	for (i = 0; i < mapinfo.nummaps; i++)
	{
		map = mapinfo.maps[i].number;
		GetEpisode(map);
		level = GetLevel(gamestate.episode, map);

		console_printf("E%dA%d - \"%s\"", gamestate.episode, level, mapinfo.maps[i].mapname);
	}

	return 0;
}

/* help */
int _cmd_help(int argc, char **argv)
{
	cmd_t *cmd;

	if (argc < 2)
	{
		cmd = cmd_list;

		while (cmd != NULL)
		{
			/* print name and help text */
			console_printf("%s: %s", cmd->name, cmd->help);

			/* move down chain */
			cmd = cmd->next;
		}

		return 0;
	}

	/* find specific cvar */
	if ((cmd = cmd_retrieve(argv[1])) != NULL)
	{
		console_printf("%s: %s", cmd->name, cmd->help);
	}
	else
	{
		console_printf("no command named \"%s\"", argv[1]);
		return 1;
	}

	return 0;
}

/* find */
int _cmd_find(int argc, char **argv)
{
	int i;
	char *ptr;
	cmd_t *cmd;
	cvar_t *cvar;

	if (argc < 2)
	{
		console_printf("must specify search string");
		return 1;
	}

	/* iterate over cmds */
	cmd = cmd_list;
	while (cmd != NULL)
	{
		/* do text search */
		if ((ptr = strstr(cmd->name, argv[1])) != NULL)
			console_printf("command: %s: %s", cmd->name, cmd->help);

		/* next */
		cmd = cmd->next;
	}

	/* iterate over cvars */
	cvar = cvar_list;
	while (cvar != NULL)
	{
		/* do text search */
		if ((ptr = strstr(cvar->name, argv[1])) != NULL)
			console_printf("cvar: %s", cvar->name);

		/* next */
		cvar = cvar->next;
	}

	return 0;
}

#if 0
/* dopefish */
int _cmd_dopefish(int argc, char **argv)
{
	extern boolean dopefish;

	if (dopefish)
	{
		dopefish = false;
		console_printf("dopefish mode DISABLED!");

		// 1998 dopefish patch
		if (W_CheckNumForName("DOPESTRT") > -1)
		{
			s_collectorfdoor1.shapenum = COLLECTOR1;
			s_collectorfdoor2.shapenum = COLLECTOR3;
			s_collectorfdoor3.shapenum = COLLECTOR5;
			s_collectorfdoor4.shapenum = COLLECTOR7;
			s_collectorfdoor5.shapenum = COLLECTOR9;
			s_collectorfdoor6.shapenum = COLLECTOR11;
			s_collectorfdoor7.shapenum = COLLECTOR13;
			s_collectorfdoor8.shapenum = COLLECTOR15;

			s_collectorwander1.shapenum = COLLECTOR1;
			s_collectorwander2.shapenum = COLLECTOR3;
			s_collectorwander3.shapenum = COLLECTOR5;
			s_collectorwander4.shapenum = COLLECTOR7;
			s_collectorwander5.shapenum = COLLECTOR9;
			s_collectorwander6.shapenum = COLLECTOR11;
			s_collectorwander7.shapenum = COLLECTOR13;
			s_collectorwander8.shapenum = COLLECTOR15;

			stats[stat_collector].picnum = COLLECTOR1;
		}
	}
	else
	{
		dopefish = true;
		console_printf("dopefish mode ENABLED!");

		// 1998 dopefish patch
		if (W_CheckNumForName("DOPESTRT") > -1)
		{
			s_collectorfdoor1.shapenum = DOPE1;
			s_collectorfdoor2.shapenum = DOPE2;
			s_collectorfdoor3.shapenum = DOPE3;
			s_collectorfdoor4.shapenum = DOPE4;
			s_collectorfdoor5.shapenum = DOPE5;
			s_collectorfdoor6.shapenum = DOPE6;
			s_collectorfdoor7.shapenum = DOPE7;
			s_collectorfdoor8.shapenum = DOPE8;

			s_collectorwander1.shapenum = DOPE1;
			s_collectorwander2.shapenum = DOPE2;
			s_collectorwander3.shapenum = DOPE3;
			s_collectorwander4.shapenum = DOPE4;
			s_collectorwander5.shapenum = DOPE5;
			s_collectorwander6.shapenum = DOPE6;
			s_collectorwander7.shapenum = DOPE7;
			s_collectorwander8.shapenum = DOPE8;

			stats[stat_collector].picnum = DOPE1;
		}
	}

	return 0;
}
#endif

/* exec */
int _cmd_exec(int argc, char **argv)
{
	if (argc < 2)
	{
		console_printf("must specify config filename");
		return 1;
	}

	/* execute the thing */
	if (console_exec(argv[1]) == false)
	{
		console_printf("failed to execute %s", argv[1]);
		return 1;
	}

	return 0;
}

/* cmdlib array */
cmd_t _cmdlib[] = {
	CMD("quit", "exit the game immediately", _cmd_quit),
	CMD("map", "load map by name", _cmd_map),
	CMD("mapset", "load mapset by filename", _cmd_mapset),
	CMD("maps", "list all maps in mapset", _cmd_maps),
	CMD("help", "print help text", _cmd_help),
	CMD("find", "find command or variable by name", _cmd_find),
// FIXME: re-add this once i merge the other stuff from ROTTEN
#if 0
	CMD("dopefish", "?", _cmd_dopefish),
#endif
	CMD("exec", "execute config script", _cmd_exec)
};

/* register standard library of cmds */
void cmdlib_init(void)
{
	for (int i = 0; i < sizeof(_cmdlib) / sizeof(cmd_t); i++)
	{
		cmd_register(&_cmdlib[i]);
	}
}

/* shutdown cmdlib */
void cmdlib_quit(void)
{
	cmd_list = NULL;
}

//****************************************************************************
//
// CONSOLE
//
//****************************************************************************

/* local constants */
#define CONSOLE_BUFFER_SIZE (8192)

/* local variables */
static struct {
	char buf[CONSOLE_BUFFER_SIZE];
	char *bufptr;
	char *lines[CONSOLE_NUM_LINES];
	int num_lines;
} console;

/* global variables */
boolean console_should_close = false;

/* push up console lines buffer with new pointer */
static void console_push_line(char *ptr)
{
	/* add pointer to lines buffer */
	console.lines[console.num_lines++] = ptr;

	/* copy second half to first half if we're about to overflow */
	if (console.num_lines >= CONSOLE_NUM_LINES - 1)
	{
		void *first;
		void *second;
		int len;

		/* pointer to first half */
		first = console.lines;

		/* pointer to second half */
		second = &console.lines[(CONSOLE_NUM_LINES / 2) - 1];

		/* copy and set size */
		len = sizeof(char *) * (CONSOLE_NUM_LINES / 2);

		/* copy second half to first half */
		memcpy(first, second, len);

		/* zero out second half (minus one for input line) */
		second = &console.lines[CONSOLE_NUM_LINES / 2];
		len = sizeof(char *) * ((CONSOLE_NUM_LINES / 2) - 1);
		memset(second, 0, len);

		/* set num_lines */
		console.num_lines = CONSOLE_NUM_LINES / 2;
	}
}

/* push string info console */
static void console_push(char *src)
{
	int i;
	int len_src = strlen(src);

	/* bounds checks */
	if (console.bufptr + len_src + 1 > console.buf + CONSOLE_BUFFER_SIZE)
		console.bufptr = console.buf;

	/* add string to text buffer */
	sprintf(console.bufptr, "%s", src);

	/* add pointer to lines buffer */
	console_push_line(console.bufptr);

	/* advance text buffer pointer */
	console.bufptr += len_src + 1;

	/* check for newlines and push line again */
	for (i = 0; i < len_src; i++)
	{
		if (console.bufptr[i] == '\n')
		{
			console.bufptr[i] = '\0';
			console_push_line(&console.bufptr[i + 1]);
		}
	}
}
/* tokenize based on semicolons and newlines */
static boolean console_tokenize_semicolons_newlines(char *s, int max_argc, char **argv, int *argc)
{
	char *ptr, *end;
	int _argc = 0;
	boolean comment = false;

	/* fail */
	if (!s || !max_argc || !argv || !argc)
		return false;

	/* set start pointer */
	ptr = s;

	/* main tokenize loop */
	for (;;)
	{
		/* skip leading semicolons and line breaks */
		while (*ptr && (*ptr == ';' || *ptr == '\n' || *ptr == '\r'))
			ptr++;

		/* skip comment characters */
		while (*ptr && *ptr == '#')
		{
			comment = true;
			ptr++;
		}

		/* reached end of string */
		if (!*ptr)
			break;

		/* set end pointer */
		end = ptr + 1;

		/* move end position until we hit a semicolon, a newline, or the end of the string */
		/* we handle comments too */
		if (comment)
		{
			/* iterate until newline */
			while (*end && !(*end == '\n' || *end == '\r'))
				end++;

			comment = false;
			ptr = end + 1;
			continue;
		}
		else
		{
			while (*end && !(*end == ';' || *end == '\n' || *end == '\r'))
				end++;
		}

		if (_argc < max_argc - 1)
			argv[_argc++] = ptr;

		/* reached end of string */
		if (!*end)
			break;

		/* set end of arg to null */
		*end = 0;

		/* set ptr for next iteration */
		ptr = end + 1;
	}

	/* null out last arg */
	argv[_argc] = 0;

	/* set out argc */
	*argc = _argc;

	/* return success */
	return true;
}

/* tokenize console into provided array */
static boolean console_tokenize_whitespace_quotes(char *s, int max_argc, char **argv, int *argc)
{
	char *ptr, *end;
	int _argc = 0;
	boolean quoted = false;

	/* fail */
	if (!s || !max_argc || !argv || !argc)
		return false;

	/* set start pointer */
	ptr = s;

	/* main tokenize loop */
	for (;;)
	{
		/* skip leading whitespace */
		while (*ptr && isspace(*ptr))
		{
			ptr++;
		}

		/* skip leading quotes */
		while (*ptr && *ptr == '"')
		{
			quoted = true;
			ptr++;
		}

		/* reached end of string */
		if (!*ptr)
			break;

		/* set end pointer */
		end = ptr + 1;

		/* push end pointer */
		if (quoted)
		{
			/* iterate until another quote mark */
			while (*end && *end != '"')
				end++;

			quoted = false;
		}
		else
		{
			while (*end && !isspace(*end))
				end++;
		}

		if (_argc < max_argc - 1)
			argv[_argc++] = ptr;

		/* reached end of string */
		if (!*end)
			break;

		/* set end of arg to null */
		*end = 0;

		/* set ptr for next iteration */
		ptr = end + 1;
	}

	/* null out last arg */
	argv[_argc] = 0;

	/* set out argc */
	*argc = _argc;

	/* return success */
	return true;
}

/* initialize developer console */
boolean console_init(void)
{
	/* initialize all memory to zero */
	memset(&console, 0, sizeof(console));

	/* set buffer pointer */
	console.bufptr = console.buf;

	return true;
}

/* shutdown console */
void console_quit(void)
{

}

/* draw console outline and current text buffer */
void console_draw(void)
{
	int i, x, y;

	/* clear region */
	EraseMenuBufRegion(CONSOLE_BOX_X, CONSOLE_BOX_Y, CONSOLE_BOX_W, CONSOLE_BOX_H);

	/* console area outline */
	DrawTMenuBufBox(CONSOLE_BOX_X, CONSOLE_BOX_Y, CONSOLE_BOX_W, CONSOLE_BOX_H);

	/* set font */
	CurrentFont = tinyfont;

	/* line start */
	x = CONSOLE_LINE_X;
	y = CONSOLE_LINE_Y;

	/* draw text */
	for (i = console.num_lines; i >= 0; i--)
	{
		/* handle null lines */
		if (console.lines[i] == NULL)
			continue;
		if (console.lines[i][0] == '\0')
			continue;

		/* gone offscreen */
		if (y < CONSOLE_BOX_Y)
			break;

		/* draw line */
		DrawMenuBufPropString(x, y, console.lines[i]);

		/* move up */
		y -= CONSOLE_LINE_H;
	}
}

/* print to console */
void console_printf(const char *s, ...)
{
	static char buf[256];
	va_list args;

	/* do vargs */
	va_start(args, s);
	vsnprintf(buf, sizeof(buf), s, args);
	va_end(args);

	/* print to standard output */
	printf("%s\n", buf);
	console_push(buf);
}

/* parse one console command */
static boolean console_parse_command(char *s)
{
	cmd_t *cmd;
	cvar_t *cvar;
	int argc;
	char *argv[128];

	/* print command */
	console_printf("> %s", s);

	/* tokenize based on whitespace and quotes */
	if (!console_tokenize_whitespace_quotes(s, 128, argv, &argc))
		Error("failed to tokenize console string");

	if (!argv[0] || !argc)
		return false;

	/* check for cheat */
	if (DoCheatCodeString(argv[0]))
		return true;

	/* check for cmd */
	if ((cmd = cmd_retrieve(argv[0])) != NULL)
	{
		cmd->func(argc, argv);
		return true;
	}

	/* check cvar */
	if ((cvar = cvar_retrieve(argv[0])) != NULL)
	{
		/* user probably wants to set it */
		if (argv[1])
		{
			/* block if protected */
			if (cvar->flags & CVAR_FLAG_PROTECTED)
			{
				console_printf("\"%s\" is a protected variable and cannot be set", cvar->name);
				return true;
			}

			/* do set */
			cvar_set(argv[0], argv[1]);

			return true;
		}
		else
		{
			/* unset by default */
			if (!(cvar->flags & CVAR_FLAG_SET) && (cvar->flags & CVAR_FLAG_NO_DEFAULT))
			{
				console_printf("value: unset");
				console_printf("default: none");
				return true;
			}

			/* print value */
			switch (cvar->type)
			{
				case CVAR_TYPE_BOOL:
					if (cvar->val.b)
						console_printf("value: true");
					else
						console_printf("value: false");
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						if (cvar->def.b)
							console_printf("default: true");
						else
							console_printf("default: false");
					break;

				case CVAR_TYPE_INT:
					console_printf("value: %d", cvar->val.i);
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						console_printf("default: %d", cvar->def.i);
					break;

				case CVAR_TYPE_UINT:
					console_printf("value: %u", cvar->val.u);
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						console_printf("default: %u", cvar->def.u);
					break;

				case CVAR_TYPE_FIXED:
					console_printf("value: %0.4f", FIXED_TO_FLOAT(cvar->val.x));
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						console_printf("default: %0.4f", FIXED_TO_FLOAT(cvar->def.x));
					break;

				case CVAR_TYPE_FLOAT:
					console_printf("value: %0.4f", cvar->val.f);
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						console_printf("default: %0.4f", cvar->def.f);
					break;

				case CVAR_TYPE_STRING:
					console_printf("value: \"%s\"", cvar->val.s);
					if (cvar->flags & CVAR_FLAG_NO_DEFAULT)
						console_printf("default: none");
					else
						console_printf("default: \"%s\"", cvar->def.s);
					break;
			}

			return true;
		}
	}

	console_printf("\"%s\" is not a valid command, variable, or cheat", s);
	return false;
}

/* evaluate string as a console input */
void console_evaluate(char *s)
{
	int argc;
	char *argv[128];
	int i;

	/* tokenize based on newlines and semicolons */
	if (!console_tokenize_semicolons_newlines(s, 128, argv, &argc))
		Error("failed to tokenize console string");

	/* tokenize each command segment */
	for (i = 0; i < argc; i++)
		console_parse_command(argv[i]);
}

/* execute console commands from filename */
boolean console_exec(char *filename)
{
	char *buffer;
	long size;
	int file;
	long length;
	char *_filename;

	_filename = FindFileByName(filename);
	if (!_filename)
		return false;

	/* read in file */
	file = SafeOpenRead(_filename);
	length = filelength(file);
	buffer = SafeMalloc(length + 1);
	SafeRead(file, buffer, length);
	close(file);

	/* add null terminator */
	buffer[length] = '\0';

	/* evaluate */
	console_evaluate(buffer);

	/* free buffers */
	free(_filename);

	return true;
}

/* write all cvars to config file */
boolean console_write_config(char *filename)
{
	char *path;
	int i;
	FILE *file;
	cvar_t *cvar;

	/* make path */
	path = FindFileByName(filename);
	if (!path)
		return false;

	/* open file */
	file = fopen(path, "wb");
	if (!file)
		return false;

	/* write configs */
	cvar = cvar_list;
	while (cvar != NULL)
	{
		if (cvar->flags & CVAR_FLAG_SET)
		{
			/* print value */
			switch (cvar->type)
			{
				case CVAR_TYPE_BOOL:
					if (cvar->val.b)
						fprintf(file, "%s true\n");
					else
						fprintf(file, "%s false\n");
					break;

				case CVAR_TYPE_INT:
					fprintf(file, "%s %d\n", cvar->name, cvar->val.i);
					break;

				case CVAR_TYPE_UINT:
					fprintf(file, "%s %u\n", cvar->name, cvar->val.u);
					break;

				case CVAR_TYPE_FIXED:
					fprintf(file, "%s %0.4f\n", cvar->name, FIXED_TO_FLOAT(cvar->val.x));
					break;

				case CVAR_TYPE_FLOAT:
					fprintf(file, "%s %0.4f\n", cvar->name, cvar->val.f);
					break;

				case CVAR_TYPE_STRING:
					fprintf(file, "%s \"%s\"\n", cvar->name, cvar->val.s);
					break;
			}
		}

		cvar = cvar->next;
	}

	fclose(file);
	free(path);

	return true;
}

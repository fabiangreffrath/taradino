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
// scriplib.c

#include <fcntl.h>
#include <string.h>

#include "rt_def.h"
#include "scriplib.h"
#include "rt_util.h"

/*
=============================================================================

						PARSING STUFF

=============================================================================
*/

char    token[MAXTOKEN];
char    name[MAXTOKEN*2];
char    scriptfilename[512];
char    *scriptbuffer,*script_p,*scriptend_p;
int     scriptline;
bool8_t endofscript;
bool8_t tokenready;                     // only TRUE if UnGetToken was just called

/*
==============
=
= LoadScriptFile
=
==============
*/

void LoadScriptFile (char *filename)
{
	long            size;

	size = LoadFile (filename, (void **)&scriptbuffer);

	snprintf(scriptfilename, sizeof(scriptfilename), "%s", filename);
	script_p = scriptbuffer;
	scriptend_p = script_p + size;
	scriptline = 1;
	endofscript = FALSE;
	tokenready = FALSE;
}


/*
==============
=
= UnGetToken
=
= Signals that the current token was not used, and should be reported
= for the next GetToken.  Note that

GetToken (TRUE);
UnGetToken ();
GetToken (FALSE);

= could cross a line boundary.
=
==============
*/

void UnGetToken (void)
{
	tokenready = TRUE;
}


/*
==============
=
= GetToken
=
==============
*/

void GetToken (bool8_t crossline)
{
	char    *token_p;

	if (tokenready)                         // is a token allready waiting?
	{
		tokenready = FALSE;
		return;
	}

	if (script_p >= scriptend_p)
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
		endofscript = TRUE;
		return;
	}

//
// skip space
//
skipspace:
	while (*script_p <= 32)
	{
		if (script_p >= scriptend_p)
		{
			if (!crossline)
            Error ("Line %i is incomplete\nin file %s\n",
                   scriptline,scriptfilename);
			endofscript = TRUE;
			return;
		}
		if (*script_p++ == '\n')
		{
			if (!crossline)
            Error ("Line %i is incomplete\nin file %s\n",
                  scriptline,scriptfilename);
			scriptline++;
		}
	}

	if (script_p >= scriptend_p)
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
		endofscript = TRUE;
		return;
	}

	if (*script_p == ';')   // semicolon is comment field
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
		while (*script_p++ != '\n')
			if (script_p >= scriptend_p)
			{
				endofscript = TRUE;
				return;
			}
		goto skipspace;
	}

//
// copy token
//
	token_p = token;

	while ( *script_p > 32 && *script_p != ';')
	{
		*token_p++ = *script_p++;
		if (script_p == scriptend_p)
			break;
		if (token_p == &token[MAXTOKEN])
         Error ("Token too large on line %i\nin file %s\n",
                 scriptline,scriptfilename);
   }

	*token_p = 0;
}



/*
==============
=
= GetTokenEOL
=
==============
*/

void GetTokenEOL (bool8_t crossline)
{
   char    *name_p;

	if (tokenready)                         // is a token allready waiting?
	{
		tokenready = FALSE;
		return;
	}

	if (script_p >= scriptend_p)
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
      endofscript = TRUE;
		return;
	}

//
// skip space
//
skipspace:
	while (*script_p <= 32)
	{
		if (script_p >= scriptend_p)
		{
			if (!crossline)
            Error ("Line %i is incomplete\nin file %s\n",
                   scriptline,scriptfilename);
			endofscript = TRUE;
			return;
		}
		if (*script_p++ == '\n')
		{
			if (!crossline)
            Error ("Line %i is incomplete\nin file %s\n",
                   scriptline,scriptfilename);
			scriptline++;
		}
	}

	if (script_p >= scriptend_p)
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
		endofscript = TRUE;
		return;
	}

	if (*script_p == ';')   // semicolon is comment field
	{
		if (!crossline)
         Error ("Line %i is incomplete\nin file %s\n",
                 scriptline,scriptfilename);
		while (*script_p++ != '\n')
			if (script_p >= scriptend_p)
			{
				endofscript = TRUE;
				return;
			}
		goto skipspace;
	}

//
// copy token
//
   name_p  = name;

	while (*script_p >= 32)
	{
		*name_p++ = *script_p++;
		if (script_p == scriptend_p)
			break;
		if (name_p == &name[MAXTOKEN*2])
         Error ("Name too large on line %i\nin file %s\n",
                 scriptline,scriptfilename);
	}

	*name_p = 0;
}



/*
==============
=
= TokenAvailable
=
= Returns TRUE if there is another token on the line
=
==============
*/

bool8_t TokenAvailable (void)
{
	char    *search_p;

	search_p = script_p;

	if (search_p >= scriptend_p)
		return FALSE;

	while ( *search_p <= 32)
	{
		if (*search_p == '\n')
			return FALSE;
		search_p++;
		if (search_p == scriptend_p)
			return FALSE;

	}

	if (*search_p == ';')
		return FALSE;

	return TRUE;
}



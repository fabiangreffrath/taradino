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
// scriplib.h

#ifndef _scriplib_public
#define _scriplib_public


#define	MAXTOKEN	128

extern	char	token[MAXTOKEN];
extern	char	name[MAXTOKEN*2];
extern	char	*scriptbuffer,*script_p,*scriptend_p;
extern	int		scriptline;
extern	bool8_t	endofscript;
extern   bool8_t tokenready;      // only TRUE if UnGetToken was just called


void LoadScriptFile (char *filename);
void GetToken (bool8_t crossline);
void GetTokenEOL (bool8_t crossline);
void UnGetToken (void);
bool8_t TokenAvailable (void);

#endif

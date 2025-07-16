/*
    Copyright (C) 1994-1995 Apogee Software, Ltd.
    Copyright (C) 2005-2014 Simon Howard
    Copyright (C) 2023 Fabian Greffrath

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the GNU General Public License for more details.
*/

#ifndef RT_DATADIR_H
#define RT_DATADIR_H

#include "m_misc2.h"

extern char *datadir;

char *GetPrefDir (void);
char *FindFileByName(const char *name);
const char **GetDataDirs(int *num);

boolean MountKexData(void);
void UnmountKexData(void);
byte *LoadKexFile(const char *filename, size_t *len);
void FreeKexFile(byte *data);

#endif

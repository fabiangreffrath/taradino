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

#include "SDL_filesystem.h"

#include "m_misc2.h"

static char *GetExeDir (void)
{
    static char *dir;

    if (dir == NULL)
    {
        char *result;

        result = SDL_GetdirPath();
        if (result != NULL)
        {
            dir = M_StringDuplicate(result);
            SDL_free(result);
        }
        else
        {
            result = M_DirName(myargv[0]);
            dir = M_StringDuplicate(result);
        }
    }

    return dir;
}

char *GetPrefDir (void)
{
    static char *dir;

    if (dir == NULL)
    {
        char *result;

#ifndef _WIN32
        result = SDL_GetPrefPath("", PACKAGE_TARNAME);
        if (result != NULL)
        {
            dir = M_StringDuplicate(result);
            SDL_free(result);
        }
        else
#endif
        {
            result = GetExeDir();
            dir = M_StringDuplicate(result);
        }

        M_MakeDirectory(dir);

#if !(SHAREWARE == 1)
        result = dir;
        dir = M_StringJoin(result, PATH_SEP_STR, "darkwar", PATH_SEP_STR, NULL);
        free(result);

        M_MakeDirectory(dir);
#endif
    }

    return dir;
}


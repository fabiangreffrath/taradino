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
#include "rt_util.h"

static char *GetExeDir (void)
{
    static char *dir;

    if (dir == NULL)
    {
        char *result;

        result = SDL_GetBasePath();
        if (result != NULL)
        {
            dir = M_StringDuplicate(result);
            SDL_free(result);
        }
        else
        {
            result = M_DirName(_argv[0]);
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

        result = dir;
#if !(SHAREWARE == 1)
        dir = M_StringJoin(result, "darkwar", PATH_SEP_STR, NULL);
#else
        dir = M_StringJoin(result, "huntbgin", PATH_SEP_STR, NULL);
#endif
        free(result);

        M_MakeDirectory(dir);
    }

    return dir;
}

char *datadir = NULL;

#define MAX_DATADIRS 16
static char *datadirs[MAX_DATADIRS] = {0};
static int num_datadirs = 0;

const char **GetDataDirs(int *num)
{
	if (num) *num = num_datadirs;
	return (const char **)datadirs;
}

static void AddDataDir(char *dir)
{
    if (num_datadirs < MAX_DATADIRS)
    {
        datadirs[num_datadirs++] = dir;
    }
}

#ifndef _WIN32
static void AddDataPath(const char *path, const char *suffix)
{
    char *left, *p, *dup_path;

    dup_path = M_StringDuplicate(path);

    // Split into individual dirs within the list.
    left = dup_path;

    for (;;)
    {
        p = strchr(left, LIST_SEP_CHAR);
        if (p != NULL)
        {
            *p = '\0';

            AddDataDir(M_StringJoin(left, suffix, NULL));
            left = p + 1;
        }
        else
        {
            break;
        }
    }

    AddDataDir(M_StringJoin(left, suffix, NULL));

    free(dup_path);
}

static void AddXdgDirs(void)
{
    char *env, *tmp_env;

    env = getenv("XDG_DATA_HOME");
    tmp_env = NULL;

    if (env == NULL)
    {
        char *homedir = getenv("HOME");
        if (homedir == NULL)
        {
            homedir = "/";
        }

        tmp_env = M_StringJoin(homedir, "/.local/share", NULL);
        env = tmp_env;
    }

    AddDataDir(M_StringJoin(env, "/games/rott", NULL));
    free(tmp_env);

    env = getenv("XDG_DATA_DIRS");
    if (env == NULL)
    {
        env = "/usr/local/share:/usr/share";
    }

    AddDataPath(env, "/games/rott");
}
#endif

static void BuildDataDirList(void)
{
    if (datadirs[0])
    {
        return;
    }

    // current directory
    AddDataDir(".");

    // executable directory
    AddDataDir(GetExeDir());

#ifdef DATADIR
    // build-time data directory
    AddDataDir(DATADIR);
#endif

#ifndef _WIN32
    AddXdgDirs();
#endif
}

char *FindFileByName(const char *name)
{
    char *path;
    char *probe;
    int i;

    // Absolute path?

    probe = M_FileCaseExists(name);
    if (probe != NULL)
    {
        return probe;
    }

    BuildDataDirList();

    for (i = 0; i < num_datadirs; i++)
    {
        path = M_StringJoin(datadirs[i], PATH_SEP_STR, name, NULL);

        probe = M_FileCaseExists(path);
        free(path);
        if (probe != NULL)
        {
            return probe;
        }
    }

    // File not found

    return NULL;
}


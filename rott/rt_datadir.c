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

#include <sys/stat.h>

#include "SDL.h"

#include "m_misc2.h"
#include "rt_util.h"

#ifndef _WIN32
#include <pwd.h>
#endif

/*
 * storefronts
 *
 * retrieve ROTT data directories from various digital storefronts
 *
 * supported:
 * - steam
 * - heroic
 * - gog
 */

#ifdef _WIN32

/* format strings for potential data paths relative to a drive */
/* TODO: use the registry to query some of these */
static const char *storefront_paths[] = {
	/* steam - classic rott */
	"\\Program Files (x86)\\Steam\\steamapps\\common\\Rise of the Triad Dark War\\Rise of the Triad - Dark War\\",
	/* steam - ludicrous edition */
	"\\Program Files (x86)\\Steam\\steamapps\\common\\Rise of the Triad - Ludicrous Edition\\",
	/* steam - ludicrous edition (assets) */
	"\\Program Files (x86)\\Steam\\steamapps\\common\\Rise of the Triad - Ludicrous Edition\\assets\\",
	/* gog - ludicrous edition */
	"\\GOG Games\\Rise of the Triad - Ludicrous Edition\\",
	/* gog - ludicrous edition (assets) */
	"\\GOG Games\\Rise of the Triad - Ludicrous Edition\\assets\\",
	/* gog - classic rott */
	"\\GOG Games\\Rise of the Triad\\",
	/* original ms-dos installers */
	"\\ROTT\\"
};

/* format strings for potential data paths relative to %USERPROFILE% */
static const char *storefront_paths_home[] = {
	/* steam - ludicrous edition (savegames) */
	"\\Saved Games\\Nightdive Studios\\Rise of the Triad - Ludicrous Edition\\",
	/* steam - ludicrous edition (user mapsets) */
	"\\Saved Games\\Nightdive Studios\\Rise of the Triad - Ludicrous Edition\\projects\\"
};

static const int num_storefront_paths_home = sizeof(storefront_paths_home) / sizeof(const char *);

#else

/* format strings for potential data paths relative to $HOME */
/* these are all the default or expected paths, if the user customized it, oh well... */
static const char *storefront_paths[] = {
	/* steam - classic rott */
	"/.steam/steam/steamapps/common/Rise of the Triad Dark War/Rise of the Triad - Dark War/",
	/* steam - ludicrous edition */
	"/.steam/steam/steamapps/common/Rise of the Triad - Ludicrous Edition/",
	/* steam - ludicrous edition (assets) */
	"/.steam/steam/steamapps/common/Rise of the Triad - Ludicrous Edition/assets/",
	/* steam - ludicrous edition (savegames) */
	"/.steam/steam/steamapps/compatdata/1421490/pfx/drive_c/users/steamuser/Saved Games/Nightdive Studios/Rise of the Triad - Ludicrous Edition/",
	/* steam - ludicrous edition (user mapsets) */
	"/.steam/steam/steamapps/compatdata/1421490/pfx/drive_c/users/steamuser/Saved Games/Nightdive Studios/Rise of the Triad - Ludicrous Edition/projects/",
	/* heroic - classic rott */
	"/Games/Heroic/Rise of the Triad/data/",
	/* gog - ludicrous edition (wine) */
	"/.wine/drive_c/GOG Games/Rise of the Triad - Ludicrous Edition/",
	/* gog - ludicrous edition (wine) (assets) */
	"/.wine/drive_c/GOG Games/Rise of the Triad - Ludicrous Edition/assets/",
	/* gog - ludicrous edition (native) */
	"/GOG Games/Rise of the Triad - Ludicrous Edition/",
	/* gog - ludicrous edition (native) (assets) */
	"/GOG Games/Rise of the Triad - Ludicrous Edition/assets/",
	/* gog - classic rott (wine) */
	"/.wine/drive_c/GOG Games/Rise of the Triad/",
	/* gog - classic rott (native) */
	"/GOG Games/Rise of the Triad Dark War/"
};

#endif

static const int num_storefront_paths = sizeof(storefront_paths) / sizeof(const char *);

/*
 *
 * we now return to your regularly scheduled programming
 *
 */

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

static void AddStorefrontDirs(void)
{
	struct stat st;
	char path[1024];

#ifndef _WIN32
	char *prefix = getenv("HOME");

	if (prefix == NULL)
	{
		struct passwd *pwd = getpwuid(getuid());

		if (pwd == NULL)
		{
			perror("getpwuid");
			return;
		}

		prefix = pwd->pw_dir;
	}
#else
	char *prefix = getenv("USERPROFILE");
	char *prefix1 = NULL;

	if (!prefix)
	{
		char *homedrive = getenv("HOMEDRIVE");
		char *homepath = getenv("HOMEPATH");

		if (homedrive && homepath)
		{
			prefix1 = M_StringJoin(homedrive, homepath, NULL);
			prefix = prefix1;
		}
	}

	if (prefix)
	{
		for (int i = 0; i < num_storefront_paths_home; i++)
		{
			M_snprintf(path, sizeof(path), "%s%s", prefix, storefront_paths_home[i]);

			if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
				AddDataDir(M_StringDuplicate(path));
		}
	}

	if (prefix1)
		free(prefix1);

	/* set prefix to a drive letter for the following code */
	prefix = "C:";
#endif

	for (int i = 0; i < num_storefront_paths; i++)
	{
		M_snprintf(path, sizeof(path), "%s%s", prefix, storefront_paths[i]);

		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
			AddDataDir(M_StringDuplicate(path));
	}
}

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

	AddStorefrontDirs();
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

//
// rott:le kpf stuff
//

#include "miniz.h"

static mz_zip_archive kpf;

byte *LoadKexFile(const char *filename, size_t *len)
{
	size_t sz = 0;
	void *data = mz_zip_reader_extract_file_to_heap(&kpf, filename, &sz, 0);
	if (!data)
	{
		fprintf(stderr, "Couldn't load file \"%s\" from RottEX.kpf: %s\n", filename, mz_zip_get_error_string(kpf.m_last_error));
		if (len) *len = 0;
		return NULL;
	}

	if (len) *len = sz;
	return data;
}

void FreeKexFile(byte *data)
{
	if (data)
		mz_free(data);
}

void UnmountKexData(void)
{
	mz_zip_reader_end(&kpf);
}

boolean MountKexData(void)
{
	char *path;

	path = FindFileByName("RottEX.kpf");
	if (!path)
		return false;

	memset(&kpf, 0, sizeof(kpf));

	if (!mz_zip_reader_init_file(&kpf, path, 0))
	{
		fprintf(stderr, "Warning: mz_zip_reader_init_file() failed on \"%s\"\n", path);
		free(path);
		return false;
	}

	// not sure if it's a good idea to use this
	atexit(UnmountKexData);

	// clean up
	free(path);
	return true;
}

//
// individual rott:le asset handling
//

#include "spng.h"

#ifndef countof
#define countof(a) (sizeof(a)/sizeof(*a))
#endif

static const char *walbs[] = {
	"WALB03",
	"WALB04",
	"WALB05",
	"WALB06",
	"WALB07",
	"WALB08",
	"WALB13",
	"WALB14",
	"WALB15",
	"WALB16",
	"WALB17",
	"WALB18",
	"WALB19",
	"WALB20",
	"WALB21",
	"WALB26",
	"WALB27",
	"WALB28",
	"WALB40",
	"WALB41",
	"WALB42",
	"WALB44",
	"WALB46",
	"WALB48",
	"WALB51",
	"WALB58",
	"WALB59",
	"WALB60",
	"WALB61",
	"WALB69",
	"WALB70",
	"WALB71"
};

boolean PrecacheBetaWalls(void)
{
	spng_ctx *ctx = spng_ctx_new(0);
	if (!ctx)
	{
		fprintf(stderr, "Failed to create spng context!\n");
		return false;
	}

	for (int i = 0; i < countof(walbs); i++)
	{
		byte *png;
		size_t len_png;
		byte *decoded;
		size_t len_decoded;
		char name[256];
		M_snprintf(name, sizeof(name), "wad/wall/%s.png", walbs[i]);
		int err;

		// load png
		png = LoadKexFile(name, &len_png);
		if (!png && !len_png)
		{
			fprintf(stderr, "Failed to load \"%s\"\n", name);
			continue;
		}

		// parse it
		spng_set_png_buffer(ctx, png, len_png);
		spng_decoded_image_size(ctx, SPNG_FMT_PNG, &len_decoded);
		spng_set_image_limits(ctx, 64, 64);

		// check if the buffer size and dimensions are valid
		// it should be an 8-bit indexed 64x64 image
		if (len_decoded != 64 * 64)
		{
			fprintf(stderr, "%s: buffer size mismatch: size=%d!=%d\n", name, (int)len_decoded, 64 * 64);
			FreeKexFile(png);
			continue;
		}

		decoded = malloc(len_decoded);
		err = spng_decode_image(ctx, decoded, len_decoded, SPNG_FMT_PNG, 0);
		if (err != 0)
		{
			fprintf(stderr, "spng_decode_image failed: %s\n", spng_strerror(err));
			free(decoded);
			FreeKexFile(png);
			continue;
		}

		free(decoded);
		FreeKexFile(png);
	}

	spng_ctx_free(ctx);
}

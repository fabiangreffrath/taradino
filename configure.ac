#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rott], [1.1.2])
AM_INIT_AUTOMAKE([foreign no-define])
AC_CONFIG_SRCDIR([rott/version.h])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES([SDL],       [sdl2])
PKG_CHECK_MODULES([SDL_mixer], [SDL2_mixer])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h stddef.h stdlib.h string.h unistd.h stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset mkdir pow strcasecmp strerror strrchr strstr strtol])

# Set to build the shareware version
AC_ARG_ENABLE([shareware],
              [AS_HELP_STRING([--enable-shareware],
                              [set to build the shareware version])],
              [AC_DEFINE([SHAREWARE])],
              [])

# Set executable file name suffix
AC_ARG_ENABLE([suffix],
              [AS_HELP_STRING([--enable-suffix],
                              [set executable file name suffix])],
              [suffix="$enableval"],
              [suffix=no])

case "$suffix" in
no|yes)
    suffix=
    ;;
*)
    # Add leading dash
    suffix=-${suffix}
    ;;
esac

SUFFIX=${suffix}
AC_SUBST([SUFFIX])

# Set data file directory
AC_ARG_ENABLE([datadir],
              [AS_HELP_STRING([--enable-datadir],
                              [set data file directory])],
              [datadir="$enableval"],
              [datadir=no])

# Remove duplicate and trailing slashes
datadir=`echo ${datadir} | tr -s '/' | sed 's|/*$||'`

case "$datadir" in
no|yes)
    ;;
*)
    AC_DEFINE_UNQUOTED([DATADIR], ["$datadir/"])
    ;;
esac

AC_CONFIG_FILES([Makefile rott/Makefile rott/audiolib/Makefile])
AC_OUTPUT

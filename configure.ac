#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.5])
AC_INIT([Tunnelblick], [0.0.1], [jasonlingle@yahoo.com])
AC_CONFIG_SRCDIR([src/tunnelblick.cxx])
AM_INIT_AUTOMAKE([1.11])
AM_SILENT_RULES(yes)
AM_CONFIG_HEADER([config.h])
# Disable Automake's braindead want to rerun ./configure if it thinks it
# needs to regenerate the entire build system
AM_MAINTAINER_MODE
PKG_PROG_PKG_CONFIG()

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
PKG_CHECK_MODULES([SDL], [sdl])
PKG_CHECK_MODULES([SDLIMG], [SDL_image])
PKG_CHECK_MODULES([GL], [gl])
PKG_CHECK_MODULES([VORBISFILE], [vorbisfile])
# We don't really need this test (the C++ compiler does the Right Thing
# automatically), but autoconf needs to know about libm to test for
# feenableexcept.
AC_SEARCH_LIBS([cos], [m])

# Checks for header files.
AC_CHECK_HEADERS([fenv.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([feenableexcept])

AC_DEFINE([PI], [3.14159265358979323846f],
    [The ratio of a circle's circumference to its diameter in your universe.])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

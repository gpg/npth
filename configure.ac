# configure.ac                                          -*- Autoconf -*-
# Copyright (C) 2011 g10 Code GmbH
# This file is part of NPTH.
#
# NPTH is free software; you can redistribute it and/or modify it
# under the terms of either
#
# - the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# or
#
# - the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at
# your option) any later version.
#
# or both in parallel, as here.
#
# NPTH is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
# Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, see <http://www.gnu.org/licenses/>.

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
min_automake_version="1.11"

# Version number: Remember to change it immediately *after* a release.
#                 Make sure to run  "svn up" and "./autogen.sh --force"
#                  before a "make dist".  See below for the LT versions.
#
# The SVN version is usually the next intended release version with
# the string "-svnNNN" appended.  The reason for this is that tests for a
# specific feature can already be done under the assumption that the
# SVN version is the most recent one in a branch.  To disable the SVN
# version for the real release, set the my_issvn macro to no.
m4_define(my_version, [0.0])
m4_define(my_issvn, [yes])

m4_define([git_revision], m4_esyscmd([git branch -v 2>/dev/null \
          | awk '/^\* / {printf "%s",$3}']))
AC_INIT([npth],
        [my_version[]m4_if(my_issvn,[yes],[-git[]git_revision])],
        [gnupg-devel@gnupg.org])

# LT Version numbers, remember to change them just *before* a release.
#   (Code changed:			REVISION++)
#   (Interfaces added/removed/changed:	CURRENT++, REVISION=0)
#   (Interfaces added:			AGE++)
#   (Interfaces removed/changed:	AGE=0)
#
LIBNPTH_LT_CURRENT=0
LIBNPTH_LT_AGE=0
LIBNPTH_LT_REVISION=0

# If the API is changed in an incompatible way: increment the next counter.
NPTH_CONFIG_API_VERSION=1
##############################################

m4_define([git_brevis],m4_esyscmd(printf "%u" 0x[]m4_substr(git_revision,0,4)))
BUILD_REVISION=m4_if(git_revision,[],[svn_revision],[git_brevis])
PACKAGE=$PACKAGE_NAME
VERSION=$PACKAGE_VERSION

AC_CONFIG_SRCDIR([src/npth.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE($PACKAGE, $VERSION)
AM_MAINTAINER_MODE
AC_CANONICAL_HOST

# Enable GNU extensions on systems that have them.
AC_GNU_SOURCE

AH_VERBATIM([_REENTRANT],
[#ifndef _REENTRANT
# define _REENTRANT 1
#endif])

# Checks for programs.
AC_PROG_CC

AC_SUBST(LIBNPTH_LT_CURRENT)
AC_SUBST(LIBNPTH_LT_AGE)
AC_SUBST(LIBNPTH_LT_REVISION)

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of this package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version of this package])

# Don't default to build static libs.
LT_PREREQ([2.2.6])
LT_INIT([win32-dll disable-static])
LT_LANG([Windows Resource])

# For now we hardcode the use of version scripts.  It would be better
# to write a test for this or even implement this within libtool.
have_ld_version_script=no
case "${host}" in
    *-*-linux*)
	have_ld_version_script=yes
        ;;
    *-*-gnu*)
	have_ld_version_script=yes
        ;;
    *-apple-darwin*)
        AC_DEFINE(_XOPEN_SOURCE, 500, Activate POSIX interface on MacOS X)
        ;;
esac

AM_CONDITIONAL(HAVE_LD_VERSION_SCRIPT, test "$have_ld_version_script" = "yes")

have_w32_system=no
have_w32ce_system=no
have_w64_system=no
case "${host}" in
    x86_64-*mingw32*)
        have_w64_system=yes
        ;;
    *-mingw32ce*)
        have_w32ce_system=yes
	;;
esac
case "${host}" in
    *-mingw32ce*|*-mingw32*)
        have_w32_system=yes
        ;;
    *)
        ;;
esac

if test "$have_w32_system" = yes; then
   AC_DEFINE(HAVE_W32_SYSTEM,1,
             [Defined if we run on any kind of W32 API based system])
   ACSUBST
fi
AM_CONDITIONAL(HAVE_W32_SYSTEM, test "$have_w32_system" = yes)

if test "$have_w64_system" = yes; then
   AC_DEFINE(HAVE_W64_SYSTEM,1,
             [Defined if we run on a 64 bit W32 API based system])
fi
AM_CONDITIONAL(HAVE_W64_SYSTEM, test "$have_w64_system" = yes)

# Checks for libraries.

if test "$have_w32_system" = no; then
  have_pthread=no
  AC_CHECK_LIB(pthread, pthread_create)
  if test "x$ac_cv_lib_pthread_pthread_create" = xyes; then
    have_pthread=yes
    AC_DEFINE(HAVE_PTHREAD, ,[Define if we have pthread.])
  fi
  AC_CHECK_FUNCS([pthread_tryjoin_np pthread_setname_np pthread_getname_np])
fi

NETLIBS=
if test "$have_w32_system" = yes; then
   if test "$have_w32ce_system" = yes; then
     NETLIBS="-lws2 $NETLIBS"
   else
     # FIXME: Check why we need to use ws2_32 and document that.
     NETLIBS="-lws2_32 $NETLIBS"
   fi
fi
AC_SUBST(NETLIBS)


# Checks for header files.
AC_CHECK_HEADERS([sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Generate values for the DLL version info
if test "$have_w32_system" = yes; then
    BUILD_TIMESTAMP=`date --iso-8601=minutes`
    changequote(,)dnl
    BUILD_FILEVERSION=`echo "$VERSION" | sed 's/\([0-9.]*\).*/\1./;s/\./,/g'`
    changequote([,])dnl
    BUILD_FILEVERSION="${BUILD_FILEVERSION}${BUILD_REVISION}"
fi
AC_SUBST(BUILD_REVISION)
AC_SUBST(BUILD_TIMESTAMP)
AC_SUBST(BUILD_FILEVERSION)

# Substitution used for npth-config
# Fixme:  This should be based on the above tests.
NPTH_CONFIG_LIBS="-lposix -lrt"
NPTH_CONFIG_CFLAGS=""
NPTH_CONFIG_HOST="$host"
AC_SUBST(NPTH_CONFIG_API_VERSION)
AC_SUBST(NPTH_CONFIG_LIBS)
AC_SUBST(NPTH_CONFIG_CFLAGS)
AC_SUBST(NPTH_CONFIG_HOST)

run_tests="yes"
AC_ARG_ENABLE(tests,
  AC_HELP_STRING([--disable-tests], [disable tests]),
         run_tests=$enableval)
AM_CONDITIONAL(RUN_TESTS, test "$run_tests" = "yes")

# Last check.
die=no
if test "$have_w32_system" = no; then
  if test "$have_pthread" = "no"; then
     die=yes
     AC_MSG_NOTICE([[
***
*** You need Pthread to build this program.
*** Normally, this library comes with your system.  On Windows, you can use:
***   http://sourceware.org/pthreads-win32/
***]])
  fi
fi

if test "$die" = "yes"; then
    AC_MSG_ERROR([[
***
*** Required libraries not found. Please consult the above messages
*** and install them before running configure again.
***]])
fi

# Checks for library functions.
AC_CHECK_FUNCS([select])

AC_CONFIG_FILES([Makefile src/Makefile w32/Makefile tests/Makefile])
AC_CONFIG_FILES(src/npth-config, chmod +x src/npth-config)
AC_CONFIG_FILES(w32/npth-config, chmod +x w32/npth-config)
AC_OUTPUT

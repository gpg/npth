# Makefile.am - Makefile for NPTH tests.
# Copyright (C) 2011 g10 Code GmbH
#
# This file is part of nPth.
#
# nPth is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# nPth is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
# Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, see <http://www.gnu.org/licenses/>.

## Process this file with automake to produce Makefile.in

TESTS = t-mutex t-thread
XFAIL_TESTS =

if HAVE_W32_SYSTEM
AM_CPPFLAGS = -I$(top_srcdir)/w32
LDADD = ../w32/libnpth.la $(LIB_CLOCK_GETTIME)
else
AM_CPPFLAGS = -I../src
LDADD = ../src/libnpth.la $(LIBSOCKET) $(LIB_CLOCK_GETTIME)
if NPTH_FORK_FAILURE
# Fork failure is expected on such a system
XFAIL_TESTS += t-fork
else
TESTS += t-fork
endif
endif

LDADD += @LDADD_FOR_TESTS_KLUDGE@
if HAVE_W32_SYSTEM
AM_LDFLAGS = -no-fast-install
else
AM_LDFLAGS = -no-install
endif

noinst_HEADERS = t-support.h

noinst_PROGRAMS = $(TESTS)

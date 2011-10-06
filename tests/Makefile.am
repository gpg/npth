# Makefile.am - Makefile for NPTH tests.
# Copyright (C) 2011 g10 Code GmbH
#
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

## Process this file with automake to produce Makefile.in

TESTS = t-mutex

AM_CPPFLAGS = -pthread -I$(top_srcdir)/src
AM_LDFLAGS = -pthread
LDADD = ../src/libnpth.la

noinst_HEADERS = t-support.h

noinst_PROGRAMS = $(TESTS)

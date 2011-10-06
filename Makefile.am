# Makefile.am - Top level Makefile for npth.
# Copyright (C) 2011 g10 Code GmbH
#
# This file is part of NPTH.
#
# NPTH is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# NPTH is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
# Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, see <http://www.gnu.org/licenses/>.

## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = dist-bzip2 no-dist-gzip

EXTRA_DIST = npth.spec.in autogen.sh

if RUN_TESTS
tests = tests
else
tests =
endif

SUBDIRS = src ${tests}

# Fix the version of the spec file and create a file named VERSION
# to be used for patch's Prereq: feature.
dist-hook:
	@set -e; \
	sed -e 's/@pkg_version@/$(VERSION)/g' \
	    $(top_srcdir)/npth.spec.in  > $(distdir)/npth.spec
	echo "$(VERSION)" > $(distdir)/VERSION

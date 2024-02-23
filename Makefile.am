# Makefile.am - Top level Makefile for npth.
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

# Location of the released tarball archives.  This is prefixed by
# the variable RELEASE_ARCHIVE in ~/.gnupg-autogen.rc.  For example:
# RELEASE_ARCHIVE=wk@somehost:archive/tarballs
RELEASE_ARCHIVE_SUFFIX = npth
# Macro to help the release target.
RELEASE_NAME = $(PACKAGE_TARNAME)-$(PACKAGE_VERSION)

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = npth.pc

EXTRA_DIST = npth-config.in npth.m4 npth.spec.in autogen.sh autogen.rc \
             HACKING VERSION npth.pc.in build-aux/libtool-patch.sed
if INSTALL_NPTH_CONFIG
bin_SCRIPTS = npth-config
else
noinst_SCRIPTS = npth-config
endif
m4datadir = $(datadir)/aclocal
m4data_DATA = npth.m4

if RUN_TESTS
tests = tests
else
tests =
endif

if HAVE_W32_SYSTEM
npthdir = w32
else
npthdir = src
endif

SUBDIRS = ${npthdir} ${tests}

# Fix the version of the spec file and create a file named VERSION
# to be used for patch's Prereq: feature.
dist-hook: gen-ChangeLog
	@set -e; \
	sed -e 's/@pkg_version@/$(VERSION)/g' \
	    $(top_srcdir)/npth.spec.in  > $(distdir)/npth.spec

distcheck-hook:
	set -e; ( \
	pref="#+macro: $$(echo $(PACKAGE_NAME)|tr '-' '_')_" ;\
	reldate="$$(date -u +%Y-%m-%d)" ;\
        echo "$${pref}ver  $(PACKAGE_VERSION)"  ;\
        echo "$${pref}date $${reldate}" ;\
        list='$(DIST_ARCHIVES)'; for i in $$list; do \
	  case "$$i" in *.tar.bz2) \
            echo "$${pref}size $$(wc -c <$$i|awk '{print int($$1/1024)}')k" ;\
	    echo "$${pref}sha1 $$(sha1sum <$$i|cut -d' ' -f1)" ;\
	    echo "$${pref}sha2 $$(sha256sum <$$i|cut -d' ' -f1)" ;;\
	  esac;\
	done ) | tee $(distdir).swdb


gen_start_date = 2011-12-01T00:00:00
.PHONY: gen-ChangeLog
gen-ChangeLog:
	if test -d $(top_srcdir)/.git; then				\
	  (cd $(top_srcdir) &&                      			\
	   gitlog-to-changelog						\
	    --append-dot --tear-off					\
	    --amend=build-aux/git-log-fix				\
	    --since=$(gen_start_date) ) > $(distdir)/cl-t;		\
          cat $(top_srcdir)/build-aux/git-log-footer >> $(distdir)/cl-t;\
	  rm -f $(distdir)/ChangeLog;					\
	  mv $(distdir)/cl-t $(distdir)/ChangeLog;			\
	fi



release:
	+(set -e;\
	 if [ "$(abs_top_builddir)" = "$(abs_top_srcdir)" ]; then \
           echo "error: build directory must not be the source directory" >&2;\
	   exit 2;\
         fi ;\
	 echo "/* Build started at $$(date -uIseconds) */" ;\
	 cd $(top_srcdir); \
	 ./autogen.sh --force; \
	 cd $(abs_top_builddir); \
	 rm -rf dist; mkdir dist ; cd dist ; \
	 $(abs_top_srcdir)/configure --enable-maintainer-mode; \
	 $(MAKE) distcheck; \
	 echo "/* Build finished at $$(date -uIseconds) */" ;\
         echo "/*" ;\
	 echo " * Please run the final step interactivly:" ;\
	 echo " *   make sign-release" ;\
	 echo " */" ;\
	) 2>&1 | tee "$(RELEASE_NAME).buildlog"

sign-release:
	 +(set -e; \
	  test $$(pwd | sed 's,.*/,,') = dist || cd dist; \
	  x=$$(grep '^[[:blank:]]*RELEASE_ARCHIVE[[:blank:]]*=' \
                    $$HOME/.gnupg-autogen.rc|cut -d= -f2|xargs);\
          if [ -z "$$x" ]; then \
             echo "error: RELEASE_ARCHIVE missing in ~/.gnupg-autogen.rc">&2; \
             exit 2;\
          fi;\
          myarchive="$$x/$(RELEASE_ARCHIVE_SUFFIX)";\
	  x=$$(grep '^[[:blank:]]*RELEASE_SIGNKEY[[:blank:]]*=' \
                     $$HOME/.gnupg-autogen.rc|cut -d= -f2|xargs);\
          if [ -z "$$x" ]; then \
             echo "error: RELEASE_SIGNKEY missing in ~/.gnupg-autogen.rc">&2; \
             exit 2;\
          fi;\
	  mysignkey="$$x";\
	  files1="$(RELEASE_NAME).tar.bz2" ; \
	  files2="$(RELEASE_NAME).tar.bz2.sig \
		  $(RELEASE_NAME).swdb \
		  $(RELEASE_NAME).buildlog" ;\
	  echo "/* Signing the source tarball ..." ;\
	  gpg -sbu $$mysignkey $(RELEASE_NAME).tar.bz2 ;\
	  cat $(RELEASE_NAME).swdb >swdb.snippet;\
	  echo >>swdb.snippet ;\
	  sha1sum $${files1} >>swdb.snippet ;\
          cat "../$(RELEASE_NAME).buildlog" swdb.snippet \
               | gzip >$(RELEASE_NAME).buildlog ;\
          echo "Copying to archive $$myarchive ..." ;\
	  scp -p $${files1} $${files2} $${myarchive}/ || true;\
	  echo '/*' ;\
	  echo ' * All done; for checksums see dist/swdb.snippet' ;\
	  echo ' */' ;\
	 )

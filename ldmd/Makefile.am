# Copyright 2009 University Corporation for Atmospheric Research
#
# This file is part of the LDM package.  See the file COPYRIGHT
# in the top-level source-directory of the package for copying and
# redistribution conditions.
#
## Process this file with automake to produce Makefile.in

LD_GEN			= @LD_GEN@
YACC			= @YACC@
EXTRA_DIST		= \
    ldmd.1 \
    ldmd.conf \
    parser.c \
    parser.y \
    scanner.c \
    scanner.l \
    wordexp.h
PQ_SUBDIR		= @PQ_SUBDIR@
BUILT_SOURCES		= parser.c scanner.c
bin_PROGRAMS		= ldmd
ldmd_SOURCES	= \
    parser.c \
    ldmd.c \
    wordexp.c
CPPFLAGS		= \
    -I$(top_srcdir)/registry \
    -I$(top_srcdir)/ulog \
    -I$(top_srcdir)/protocol \
    -I$(top_srcdir)/pq \
    -I$(top_srcdir)/misc \
    -I$(top_srcdir)
LDADD			= \
    $(top_builddir)/lib/libldm.la \
    $(top_builddir)/libxml2/libxml2.la \
    $(LD_GEN) \
    -lm
dist_man1_MANS		= ldmd.1
TAGS_FILES		= \
    ../$(PQ_SUBDIR)/*.c ../$(PQ_SUBDIR)/*.h \
    ../protocol/*.c ../protocol/*.h \
    ../ulog/*.c ../ulog/*.h \
    ../misc/*.c ../misc/*.h \
    ../registry/*.c ../registry/*.h \
    ../rpc/*.c ../rpc/*.h
CLEANFILES		= *.i .*.info

if MAINTAINER
ldmd.1:		ldmd.1.in
	$(top_srcdir)/registry/substPaths <$< >$@.tmp
	mv $@.tmp $@
endif

if MAINTAINER

parser.c:	parser.y
	status=1; \
	$(YACC) parser.y && \
	    echo '#include "config.h"' >parser.c && \
	    cat y.tab.c >>parser.c && \
	    status=0; \
	rm -f y.tab.c conf.tab.c; \
	test $$status -ne 0 && rm -f $@; \
	exit $$status

# NOTE: flex(1) is used instead of lex(1) in order to generate a scanner that
# can process "include" statements.
#
scanner.c:	scanner.l
	flex scanner.l
	mv lex.yy.c scanner.c

endif

install-data-local:    \
    $(DESTDIR)$(sysconfdir) $(DESTDIR)$(sysconfdir)/ldmd.conf
$(DESTDIR)$(sysconfdir)/ldmd.conf:
	$(INSTALL_DATA) $(srcdir)/ldmd.conf $@

$(DESTDIR)$(sysconfdir):
	mkdir $@

uninstall-local:
	rm -f $(DESTDIR)$(mandir)/man1/ldmd.1

valgrind:	ldmd
	$(TESTS_ENVIRONMENT) $(LIBTOOL) --mode=execute valgrind \
	    --leak-check=full --show-reachable=yes \
	    --trace-children=no --child-silent-after-fork=no ./ldmd -l-

debug:		ldmd
	$(TESTS_ENVIRONMENT) $(LIBTOOL) --mode=execute gdb ./ldmd

.c.i:
	$(COMPILE) $(CPPFLAGS) -E -o $@ $<
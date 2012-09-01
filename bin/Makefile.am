################################################################################
# Copyright (c) 2012 Los Alamos National Security, LLC
# All rights reserved.
################################################################################

#------------------------------------------------------------------------------#
# Makefile.am for bin directory
#------------------------------------------------------------------------------#

bin_PROGRAMS = \
	ska

bin_SCRIPTS = \
	skair

CLEANFILES = ${bin_SCRIPTS}

INCLUDES = -I${top_srcdir}/src/include \
	@EXTRA_CPPFLAGS@

ska_SOURCES = \
	@top_srcdir@/bin/ska.cc
ska_LDFLAGS = @EXTRA_LDFLAGS@
ska_LDADD = @EXTRA_LIBS@  ${top_builddir}/lib/libska.la

################################################################################
# Copyright (c) 2012 Los Alamos National Security, LLC
# All rights reserved.
################################################################################

#------------------------------------------------------------------------------#
# Makefile.am for sandbox directory
#------------------------------------------------------------------------------#

bin_PROGRAMS = \
	dag

INCLUDES = -I${top_srcdir}/src/include \
	@EXTRA_CPPFLAGS@

dag_SOURCES = \
	${top_srcdir}/sandbox/dag.cc
dag_LDFLAGS = @EXTRA_LDFLAGS@
dag_LDADD = @EXTRA_LIBS@  ${top_builddir}/lib/libska.la

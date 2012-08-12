################################################################################
# Copyright (c) 2012 Los Alamos National Security, LLC
# All rights reserved.
################################################################################

#------------------------------------------------------------------------------#
# Makefile.am for viewer directory
#------------------------------------------------------------------------------#

bin_PROGRAMS = \
	skaview

INCLUDES = -I${top_srcdir}/viewer \
	@VIEWER_CPPFLAGS@

moc_%.cc: %.hh
	@MOC@ $< -o $@

qrc_%.cc: %.qrc
	@RCC@ $< -o $@

skaview_SOURCES = \
	@top_srcdir@/viewer/skaview.cc \
	@top_srcdir@/viewer/ViewMain.cc \
	@top_srcdir@/viewer/ViewPipeline.cc
nodist_skaview_SOURCES = \
	moc_ViewMain.cc \
	moc_ViewPipeline.cc \
	qrc_skaview.cc
skaview_LDFLAGS = @VIEWER_LDFLAGS@
skaview_LDADD = @VIEWER_LIBS@

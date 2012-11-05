################################################################################
# Copyright (c) 2012 Los Alamos National Security, LLC
# All rights reserved.
################################################################################

#------------------------------------------------------------------------------#
# Makefile.am for viewer directory
#------------------------------------------------------------------------------#

bin_PROGRAMS = \
	skaview

AM_CPPFLAGS = -I${top_srcdir}/viewer \
	@VIEWER_CPPFLAGS@

EXTRA_DIST = \
	${top_srcdir}/viewer/skaview.qrc \
	${top_srcdir}/viewer/icons

moc_%.cc: %.hh
	@MOC@ $< -o $@

qrc_%.cc: %.qrc ${top_srcdir}/viewer/icons/*.png
	@RCC@ $< -o $@

skaview_SOURCES = \
	${top_srcdir}/viewer/ViewCycle.hh \
	${top_srcdir}/viewer/ViewData.hh \
	${top_srcdir}/viewer/ViewHighlight.hh \
	${top_srcdir}/viewer/ViewInstruction.hh \
	${top_srcdir}/viewer/ViewIssue.hh \
	${top_srcdir}/viewer/ViewMain.hh \
	${top_srcdir}/viewer/ViewPipeline.hh \
	${top_srcdir}/viewer/ViewSlope.hh \
	${top_srcdir}/viewer/ViewUtils.hh \
	${top_srcdir}/viewer/ViewMain.cc \
	${top_srcdir}/viewer/ViewPipeline.cc \
	${top_srcdir}/viewer/ViewSlope.cc \
	${top_srcdir}/viewer/skaview.cc

nodist_skaview_SOURCES = \
	moc_ViewMain.cc \
	moc_ViewPipeline.cc \
	moc_ViewSlope.cc \
	qrc_skaview.cc

skaview_LDFLAGS = @VIEWER_LDFLAGS@
skaview_LDADD = @VIEWER_LIBS@

clean-local:
	rm -rf ${top_builddir}/viewer/qrc_skaview.cc

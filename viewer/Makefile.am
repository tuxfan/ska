#------------------------------------------------------------------------------#
# Makefile.am for viewer directory
#------------------------------------------------------------------------------#

bin_PROGRAMS = \
	skaview

INCLUDES = -I${top_srcdir}/viewer \
	@VIEWER_CPPFLAGS@

moc_%.cc: %.h
	@MOC@ $< -o $@

skaview_SOURCES = \
	@top_srcdir@/viewer/ViewMain.cc \
	@top_srcdir@/viewer/ViewPipeline.cc \
	@top_srcdir@/viewer/skaview.cc
nodist_skaview_SOURCES = \
	moc_ViewMain.cc \
	moc_ViewPipeline.cc
skaview_LDFLAGS = @VIEWER_LDFLAGS@
skaview_LDADD = @VIEWER_LIBS@

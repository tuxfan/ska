################################################################################
# Copyright (c) 2012 Los Alamos National Security, LLC
# All rights reserved.
################################################################################

#------------------------------------------------------------------------------#
# Top-level makefile for SKA project
#------------------------------------------------------------------------------#

SUBDIRS = lib bin viewer @SANDBOX@

#------------------------------------------------------------------------------#
# Create links
#------------------------------------------------------------------------------#

links:
	@(cd ${top_srcdir}/src; [ -d include ] && rm -rf include; \
		mkdir include && cd include; \
		for file in `find .. -type f -regex '.*\.h*'` ; do \
			link=`echo $$file`; \
			ln -s $$link .; \
		done \
	)

#------------------------------------------------------------------------------#
# Extra install data
#------------------------------------------------------------------------------#

install-data-hook:
	@(mkdir -p ${datarootdir}/ska && \
		cp -r ${top_srcdir}/architecture ${datarootdir}/ska && \
		cp -r ${top_srcdir}/sample ${datarootdir}/ska)

#------------------------------------------------------------------------------#
# Extra install data
#------------------------------------------------------------------------------#

dist-hook:
	@(cp -r ${top_srcdir}/architecture ${distdir} && \
		cp -r ${top_srcdir}/sample ${distdir} && \
		cp -r ${top_srcdir}/machine ${distdir})

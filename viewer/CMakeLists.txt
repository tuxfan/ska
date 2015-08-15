#~----------------------------------------------------------------------------~#
# Copyright (c) 2014 Los Alamos National Security, LLC
# All rights reserved.
#~----------------------------------------------------------------------------~#

include(${QT_USE_FILE})

#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#

QT4_WRAP_CPP(MOCSrcs
  ViewPipeline.h
  ViewMain.h
  ViewGraph.h
  ViewSlope.h
)

#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#

QT4_ADD_RESOURCES(RCSrcs skaview.qrc)

#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

#------------------------------------------------------------------------------#
# Add a rule to build the executable
#------------------------------------------------------------------------------#

add_executable(skaview
  skaview.cc
  ViewGraph.cc
  ViewMain.cc
  ViewPipeline.cc
  ViewSlope.cc
  ${MOCSrcs}
)

target_link_libraries(skaview
  ${QT_LIBRARIES}
  ${QWT_LIBRARIES}
  ${GRAPHVIZ_LIBRARIES}
)

#----------------------------------------------------------------------------~-#
# Formatting options for vim.
# vim: set tabstop=2 shiftwidth=2 expandtab :
#----------------------------------------------------------------------------~-#

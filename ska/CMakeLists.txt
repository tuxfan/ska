#~----------------------------------------------------------------------------~#
# Copyright (c) 2014 Los Alamos National Security, LLC
# All rights reserved.
#~----------------------------------------------------------------------------~#

#------------------------------------------------------------------------------#
# Add a rule to build the executable
#------------------------------------------------------------------------------#

add_executable(skabin ska.cc)
set_target_properties(skabin PROPERTIES OUTPUT_NAME ska)

#------------------------------------------------------------------------------#
# Add include dependencies
#------------------------------------------------------------------------------#

target_include_directories(skabin PRIVATE ${LLVM_INCLUDE_DIRS})
target_include_directories(skabin PRIVATE ${XercesC_INCLUDE_DIRS})
target_include_directories(skabin PRIVATE ${GRAPHVIZ_INCLUDE_DIRS})

#------------------------------------------------------------------------------#
# Add link dependencies
#------------------------------------------------------------------------------#

target_link_libraries(skabin ${CINCH_LINK_LIBRARIES}
  ${LLVM_LIBRARIES}
  ${XercesC_LIBRARIES}
  ${GRAPHVIZ_LIBRARIES}
  pthread dl tinfo)

#------------------------------------------------------------------------------#
# Create skair script
#------------------------------------------------------------------------------#

endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  configure_file(${CMAKE_SOURCE_DIR}/ska/skair.in
    ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/skair)
  file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/skair
    DESTINATION ${CMAKE_BINARY_DIR}/bin
    FILE_PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )

  install(FILES ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/skair
    DESTINATION bin
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

#~---------------------------------------------------------------------------~-#
# Formatting options for emacs and vim.
# vim: set tabstop=2 shiftwidth=2 expandtab :
#~---------------------------------------------------------------------------~-#

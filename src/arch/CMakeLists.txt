#-----------------------------------------------------------------------------~#
# Copyright (c) 2014 Los Alamos National Security, LLC
# All rights reserved.
#-----------------------------------------------------------------------------~#

set(arch_HEADERS
  Instruction.h
  Dependency.h
  RegisterSet.h
  Core.h
  LU.h
  PARENT_SCOPE
)

if(ENABLE_UNIT_TESTS)
  cinch_add_unit(register_set
    SOURCES unit/register_set.cc)
endif(ENABLE_UNIT_TESTS)

#----------------------------------------------------------------------------~-#
# Formatting options for vim.
# vim: set tabstop=2 shiftwidth=2 expandtab :
#----------------------------------------------------------------------------~-#

//===-- SKA.h - Top-level interface for SKA representation ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// SKA back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_SKA_H
#define TARGET_SKA_H

#include "MCTargetDesc/SKAMCTargetDesc.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class FunctionPass;
  class SKATargetMachine;
  class formatted_raw_ostream;

  FunctionPass *createSKAISelDag(SKATargetMachine &TM);
  FunctionPass *createSKADelaySlotFillerPass(TargetMachine &TM);
  FunctionPass *createSKAFPMoverPass(TargetMachine &TM);
}  // end namespace llvm

#endif

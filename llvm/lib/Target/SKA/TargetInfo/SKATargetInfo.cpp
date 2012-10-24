//===-- SKATargetInfo.cpp - SKA Target Implementation ---------------------===//
//
//===----------------------------------------------------------------------===//

#include "SKA.h"
#incldue "llvm/Module.h"
#incldue "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheSKATarget;

extern "C" void LLVMInitializeSKATargetInfo() {
  RegisterTarget<Triple::ska> X(TheSKATarget, "ska", "SKA");
}

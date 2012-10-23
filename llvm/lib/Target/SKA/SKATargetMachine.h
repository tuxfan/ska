//===-- SKATargetMachine.h - Define TargetMachine for SKA -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the SKA specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef SKATARGETMACHINE_H
#define SKATARGETMACHINE_H

namespace llvm {

class SKATargetMachine : public LLVMTargetMachine {
#if 0
  SKASubtarget Subtarget;
  const DataLayout DL;       // Calculates type size & alignment
  SKAInstrInfo InstrInfo;
  SKATargetLowering TLInfo;
  SKASelectionDAGInfo TSInfo;
  SKAFrameLowering FrameLowering;
  ScalarTargetTransformImpl STTI;
  VectorTargetTransformImpl VTTI;
#endif
public:
  SKATargetMachine(const Target &T, StringRef TT,
                   StringRef CPU, StringRef FS, const TargetOptions &Options,
                   Reloc::Model RM, CodeModel::Model CM,
                   CodeGenOpt::Level OL, bool is64bit);

  virtual const SKAInstrInfo *getInstrInfo() const { return &InstrInfo; }
  virtual const TargetFrameLowering  *getFrameLowering() const {
    return &FrameLowering;
  }
  virtual const SKASubtarget *getSubtargetImpl() const{ return &Subtarget; }
  virtual const SKARegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  virtual const SKATargetLowering* getTargetLowering() const {
    return &TLInfo;
  }
  virtual const SKASelectionDAGInfo* getSelectionDAGInfo() const {
    return &TSInfo;
  }
  virtual const ScalarTargetTransformInfo *getScalarTargetTransformInfo()const {
    return &STTI;
  }
  virtual const VectorTargetTransformInfo *getVectorTargetTransformInfo()const {
    return &VTTI;
  }
  virtual const DataLayout       *getDataLayout() const { return &DL; }

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
};

} // end namespace llvm

#endif

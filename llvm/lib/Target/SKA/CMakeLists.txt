set(LLVM_TARGET_DEFINITIONS SKA.td)

tablegen(LLVM SKAGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SKAGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SKAGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SKAGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SKAGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM SKAGenCallingConv.inc -gen-callingconv)
add_public_tablegen_target(SKACommonTableGen)

add_llvm_target(SKACodeGen
  SKAAsmPrinter.cpp
  SKAInstrInfo.cpp
  SKAISelDAGToDAG.cpp
  SKAISelLowering.cpp
  SKAFrameLowering.cpp
  SKAMachineFunctionInfo.cpp
  SKARegisterInfo.cpp
  SKASubtarget.cpp
  SKATargetMachine.cpp
  SKASelectionDAGInfo.cpp
  )

add_dependencies(LLVMSKACodeGen intrinsics_gen)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

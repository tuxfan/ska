#ifndef OpCodes_hh
#define OpCodes_hh

#include <map>

#include <llvm/Instructions.h>

namespace ska {

std::map<std::string, unsigned> code_map = {
	// Binary operators
	{ "add",		llvm::Instruction::Add },
	{ "fadd",	llvm::Instruction::FAdd },
	{ "sub",		llvm::Instruction::Sub },
	{ "fsub",	llvm::Instruction::FSub },
	{ "mul",		llvm::Instruction::Mul },
	{ "fmul",	llvm::Instruction::FMul },
	{ "udiv",	llvm::Instruction::UDiv },
	{ "sdiv",	llvm::Instruction::SDiv },
	{ "fdiv",	llvm::Instruction::FDiv },
	{ "urem",	llvm::Instruction::URem },
	{ "srem",	llvm::Instruction::SRem },
	{ "frem",	llvm::Instruction::FRem },

	// Logical operators
	
	// Memory operators
	{ "alloca",				llvm::Instruction::Alloca },
	{ "load",				llvm::Instruction::Load },
	{ "store",				llvm::Instruction::Store },
	{ "getelementptr",	llvm::Instruction::GetElementPtr },

	// Other operators
	{ "phi",	llvm::Instruction::PHI }
};

} // namespace ska

#endif // OpCodes_hh

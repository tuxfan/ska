/*----------------------------------------------------------------------------*
 * std::string => LLVM opcode
 *----------------------------------------------------------------------------*/

#ifndef OpCodes_hh
#define OpCodes_hh

#include <map>

#include <llvm/Instructions.h>

namespace ska {

std::map<std::string, unsigned> code_map = {
	// Terminator operators
	{ "ret",				llvm::Instruction::Ret },
	{ "br",				llvm::Instruction::Br },
	{ "switch",			llvm::Instruction::Switch },
	{ "indirectbr",	llvm::Instruction::IndirectBr },
	{ "invoke",			llvm::Instruction::Invoke },
	{ "resume",			llvm::Instruction::Resume },
	{ "unreachable",	llvm::Instruction::Unreachable },

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
	{ "shl",		llvm::Instruction::Shl },
	{ "lshr",	llvm::Instruction::LShr },
	{ "ashr",	llvm::Instruction::AShr },
	{ "and",		llvm::Instruction::And },
	{ "or",		llvm::Instruction::Or },
	{ "xor",		llvm::Instruction::Xor },
	
	// Memory operators
	{ "alloca",				llvm::Instruction::Alloca },
	{ "load",				llvm::Instruction::Load },
	{ "store",				llvm::Instruction::Store },
	{ "getelementptr",	llvm::Instruction::GetElementPtr },
	{ "fence",				llvm::Instruction::Fence },
	{ "atomiccmpxchg",	llvm::Instruction::AtomicCmpXchg },
	{ "atomicrmw",			llvm::Instruction::AtomicRMW },

	// Cast operators
	{ "trunc",		llvm::Instruction::Trunc },
	{ "zext",		llvm::Instruction::ZExt },
	{ "sext",		llvm::Instruction::SExt },
	{ "fptoui",		llvm::Instruction::FPToUI },
	{ "fptosi",		llvm::Instruction::FPToSI },
	{ "uitofp",		llvm::Instruction::UIToFP },
	{ "sitofp",		llvm::Instruction::SIToFP },
	{ "fptrunc",	llvm::Instruction::FPTrunc },
	{ "fpext",		llvm::Instruction::FPExt },
	{ "ptrtoint",	llvm::Instruction::PtrToInt },
	{ "inttoptr",	llvm::Instruction::IntToPtr },
	{ "bitcast",	llvm::Instruction::BitCast },

	// Other operators
	{ "icmp",				llvm::Instruction::ICmp },
	{ "fcmp",				llvm::Instruction::FCmp },
	{ "phi",					llvm::Instruction::PHI },
	{ "call",				llvm::Instruction::Call },
	{ "select",				llvm::Instruction::Select },
	{ "userop1",			llvm::Instruction::UserOp1 },
	{ "userop2",			llvm::Instruction::UserOp2 },
	{ "vaarg",				llvm::Instruction::VAArg },
	{ "extractelement",	llvm::Instruction::ExtractElement },
	{ "insertelement",	llvm::Instruction::InsertElement },
	{ "shufflevector",	llvm::Instruction::ShuffleVector },
	{ "extractvector",	llvm::Instruction::ExtractValue },
	{ "insertvalue",		llvm::Instruction::InsertValue },
	{ "landingpad",		llvm::Instruction::LandingPad }
};

} // namespace ska

#endif // OpCodes_hh

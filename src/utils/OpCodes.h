/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * std::string => LLVM opcode
 *----------------------------------------------------------------------------*/

#ifndef OpCodes_h
#define OpCodes_h

#include <map>

#include <llvm/IR/Instructions.h>

namespace ska {

bool is_memory_access_op(unsigned opcode) {
#if 0
	return opcode >= llvm::Instruction::Alloca &&
		opcode < llvm::Instruction::Trunc;
#endif
	return (opcode >= llvm::Instruction::Alloca &&
		opcode < llvm::Instruction::Store) ||
		opcode == llvm::Instruction::GetElementPtr;
} // is_memory_op

bool is_call_op(unsigned opcode) {
	return opcode == llvm::Instruction::Call;
} // is_call_op

bool is_phi_op(unsigned opcode) {
	return opcode == llvm::Instruction::PHI;
} // is_phi_op

#if 1
bool is_pruned_op(unsigned opcode) {
	return opcode == llvm::Instruction::GetElementPtr;
} // is_pruned_op
#endif

unsigned dummy_op() {
	return unsigned(llvm::Instruction::LandingPad) + 1;
} // dummy_op

std::map<std::string, unsigned> code_map = {
	// Terminator operators
	{ "ret",				llvm::Instruction::Ret }, // 1
	{ "br",				llvm::Instruction::Br }, // 2
	{ "switch",			llvm::Instruction::Switch }, // 3
	{ "indirectbr",	llvm::Instruction::IndirectBr }, // 4
	{ "invoke",			llvm::Instruction::Invoke }, // 5
	{ "resume",			llvm::Instruction::Resume }, // 6
	{ "unreachable",	llvm::Instruction::Unreachable }, // 6

	// Binary operators
	{ "add",		llvm::Instruction::Add }, // 8
	{ "fadd",	llvm::Instruction::FAdd }, // 9
	{ "sub",		llvm::Instruction::Sub }, // 10
	{ "fsub",	llvm::Instruction::FSub }, // 11
	{ "mul",		llvm::Instruction::Mul }, // 12
	{ "fmul",	llvm::Instruction::FMul }, // 13
	{ "udiv",	llvm::Instruction::UDiv }, // 14
	{ "sdiv",	llvm::Instruction::SDiv }, // 15
	{ "fdiv",	llvm::Instruction::FDiv }, // 16
	{ "urem",	llvm::Instruction::URem }, // 17
	{ "srem",	llvm::Instruction::SRem }, // 18
	{ "frem",	llvm::Instruction::FRem }, // 19

	// Logical operators
	{ "shl",		llvm::Instruction::Shl }, // 20
	{ "lshr",	llvm::Instruction::LShr }, // 21
	{ "ashr",	llvm::Instruction::AShr }, // 22
	{ "and",		llvm::Instruction::And }, // 23
	{ "or",		llvm::Instruction::Or }, // 24
	{ "xor",		llvm::Instruction::Xor }, // 25
	
	// Memory operators
	{ "alloca",				llvm::Instruction::Alloca }, // 26
	{ "load",				llvm::Instruction::Load }, // 27
	{ "store",				llvm::Instruction::Store }, // 28
	{ "getelementptr",	llvm::Instruction::GetElementPtr }, // 29
	{ "fence",				llvm::Instruction::Fence }, // 30
	{ "atomiccmpxchg",	llvm::Instruction::AtomicCmpXchg }, // 31
	{ "atomicrmw",			llvm::Instruction::AtomicRMW }, // 32

	// Cast operators
	{ "trunc",		llvm::Instruction::Trunc }, // 33
	{ "zext",		llvm::Instruction::ZExt }, // 34
	{ "sext",		llvm::Instruction::SExt }, // 35
	{ "fptoui",		llvm::Instruction::FPToUI }, // 36
	{ "fptosi",		llvm::Instruction::FPToSI }, // 37
	{ "uitofp",		llvm::Instruction::UIToFP }, // 38
	{ "sitofp",		llvm::Instruction::SIToFP }, // 39
	{ "fptrunc",	llvm::Instruction::FPTrunc }, // 40
	{ "fpext",		llvm::Instruction::FPExt }, // 41
	{ "ptrtoint",	llvm::Instruction::PtrToInt }, // 42
	{ "inttoptr",	llvm::Instruction::IntToPtr }, // 43
	{ "bitcast",	llvm::Instruction::BitCast }, // 44

	// Other operators
	{ "icmp",				llvm::Instruction::ICmp }, // 45
	{ "fcmp",				llvm::Instruction::FCmp }, // 46
	{ "phi",					llvm::Instruction::PHI }, // 47
	{ "call",				llvm::Instruction::Call }, // 48
	{ "select",				llvm::Instruction::Select }, // 49
	{ "userop1",			llvm::Instruction::UserOp1 }, // 50
	{ "userop2",			llvm::Instruction::UserOp2 }, // 51
	{ "vaarg",				llvm::Instruction::VAArg }, // 52
	{ "extractelement",	llvm::Instruction::ExtractElement }, // 53
	{ "insertelement",	llvm::Instruction::InsertElement }, // 54
	{ "shufflevector",	llvm::Instruction::ShuffleVector }, // 55
	{ "extractvalue",		llvm::Instruction::ExtractValue }, // 56
	{ "insertvalue",		llvm::Instruction::InsertValue }, // 57
	{ "landingpad",		llvm::Instruction::LandingPad }, // 58

	// Call operators
	/* These are not part of the normal LLVM IR instruction set.
		For now, we need them to be able to map call functions
		to the appropriate LU.  In the future, we may move to
		a more complex representation where instructions run
		across multiple LUs. */
	{ "malloc",				llvm::Instruction::LandingPad + 1 },
	{ "free",				llvm::Instruction::LandingPad + 2 },
	{ "printf",				llvm::Instruction::LandingPad + 3 },
	{ "rand",				llvm::Instruction::LandingPad + 4 },
	{ "pow",					llvm::Instruction::LandingPad + 5 },
	{ "log",					llvm::Instruction::LandingPad + 6 },

	{ "fabs",				llvm::Instruction::LandingPad + 7 },
	{ "fabsf",				llvm::Instruction::LandingPad + 7 },

	{ "sqrt",				llvm::Instruction::LandingPad + 8 },
	{ "sqrtf",				llvm::Instruction::LandingPad + 8 },

	{ "rsqrt",				llvm::Instruction::LandingPad + 9 },
	{ "hadd",				llvm::Instruction::LandingPad + 10 }
};

std::string code_name(unsigned code) {
	std::string name("unknown");
	for(auto c = code_map.begin(); c != code_map.end(); ++c) {
		if(c->second == code) {
			name = c->first;
		} // if
	} // for

	return name;
} // code_name

std::string call_name(const std::string & call) {
	return call.substr(0, call.find_first_of("("));
} // call_name

} // namespace ska

#endif // OpCodes_h

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/

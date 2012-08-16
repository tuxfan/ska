/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

#ifndef Simulator_hh
#define Simulator_hh

#if defined(USE_MANGLED_CALL_NAMES)
#include <cxxabi.h>
#endif

#include <string>
#include <cstring>
#include <vector>
#include <list>

#include <llvm/Module.h>
#include <llvm/Constants.h>
#include <llvm/Function.h>
#include <llvm/Instruction.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Support/InstIterator.h>

#include <Instruction.hh>
#include <MachineState.hh>
#include <Statistics.hh>
#include <OpCodes.hh>
#include <OpTypes.hh>
#include <Core.hh>
#include <Utils.hh>
#include <x86CallMap.hh>

namespace ska {

#ifndef SKA_VERSION_TAG
#define SKA_VERSION_TAG "undefined"
#endif

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

class simulator_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Public types
	 *-------------------------------------------------------------------------*/

	typedef std::map<llvm::Value *, instruction_t *> instruction_map_t;
	typedef std::vector<instruction_t *> instruction_vector_t;
	typedef std::list<instruction_t *> instruction_list_t;

	/*-------------------------------------------------------------------------*
	 * Constructor
	 *-------------------------------------------------------------------------*/

	simulator_t(const char * ir_file, std::ostream & stream);

	/*-------------------------------------------------------------------------*
	 * Destructor
	 *-------------------------------------------------------------------------*/

	~simulator_t() {}

private:

	/*-------------------------------------------------------------------------*
	 * Decode an llvm::Instruction.  This gets property information about
	 * the instruction (latency, issue latency, opcode, optype) that is used
	 * to contruct a instruction_t instance.
	 *-------------------------------------------------------------------------*/

	instruction_properties_t decode(llvm::Instruction * instruction);
	
	/*-------------------------------------------------------------------------*
	 * Update statistic based on the execution of 'instruction'.
	 *-------------------------------------------------------------------------*/

	void update_stats(llvm::Instruction * instruction);

	/*-------------------------------------------------------------------------*
	 * Compute the number of bytes associated with a given llvm::Type.  This
	 * is used to compute load and store sizes.
	 *-------------------------------------------------------------------------*/

	size_t bytes(llvm::Type * type);

	// private data members
	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

}; // class simulator_t

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

simulator_t::simulator_t(const char * ir_file, std::ostream & stream)
	: llvm_module_(nullptr)
{
	parameters_t & arch = parameters_t::instance();
	machine_state_t & machine = machine_state_t::instance();
	statistics_t & stats = statistics_t::instance();
	instruction_map_t processed;

	/*-------------------------------------------------------------------------*
	 * Write header information.
	 *-------------------------------------------------------------------------*/

	stream << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;
	stream << "# Static Kernel Analyzer (SKA)" << std::endl;
	stream << "# Header Section" << std::endl;
	stream << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;
	stream << "KEYWORD_SKA_VERSION " <<
		DEFINE_TO_STRING(SKA_VERSION) << std::endl;

	std::string architecture;
	arch.getval(architecture, "name");
	stream << "KEYWORD_ARCHITECTURE " << architecture <<
		std::endl << std::endl;
	
	/*-------------------------------------------------------------------------*
	 * Initialize core.
	 *-------------------------------------------------------------------------*/
	
	size_t max_issue;
	arch.getval(max_issue, "core::max_issue");
	core_t core(max_issue);

	size_t lus;
	arch.getval(lus, "lus");

	for(size_t i(0); i<lus; ++i) {
		lu_t * lu = new lu_t(i);

		// parse instructions that this LU can handle
		char key[256];
		std::string tmp;

		sprintf(key, "lu::%d::instructions", int(i));
		arch.getval(tmp, key);

		char * ops = new char[tmp.size()+1];
		strcpy(ops, tmp.c_str());

		char * tok = strtok(ops, " ");

		while(tok != nullptr) {
			lu->add_op(code_map[tok]);
			tok = strtok(nullptr, " ");
		} // while

		delete[] ops;
		tok = nullptr;

		// parse types that this LU can handle
		sprintf(key, "lu::%d::types", int(i));
		arch.getval(tmp, key);

		char * types = new char[tmp.size()+1];
		strcpy(types, tmp.c_str());

		tok = strtok(types, " ");

		while(tok != nullptr) {
			lu->add_type(type_map[tok]);
			tok = strtok(nullptr, " ");
		} // while

		delete[] types;

		// add unit to the core
		core.add_unit(lu);
	} // for

	/*-------------------------------------------------------------------------*
	 * Get parsed LLVM IR.
	 *-------------------------------------------------------------------------*/

	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	if(llvm_module_ == nullptr) {
		ExitOnError("LLVM parse failed on " << ir_file << std::endl <<
			llvm_err_.getMessage(), ska::LLVMError);
	} // if

	/*-------------------------------------------------------------------------*
	 * Visit modules.
	 *-------------------------------------------------------------------------*/

	for(llvm::Module::iterator fita = llvm_module_->begin();
		fita != llvm_module_->end(); ++fita) {

#if 0
for(llvm::Function::iterator bita = fita->begin();
	bita != fita->end(); ++bita) {
	llvm::errs() << "Basic Block: " << bita->getName() << " has " <<
		bita->size() << " instructions\n";
} // for
#endif

		llvm::inst_iterator iita = inst_begin(fita);
		
		if(iita == inst_end(fita)) {
			continue;
		} // if

		stream << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		stream << "# Module Section: " << fita->getName().str() << std::endl;
		stream << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		stream << "BEGIN_MODULE" << std::endl;
		stream << "KEYWORD_NAME " << fita->getName().str() << std::endl;

		instruction_list_t active;
		instruction_vector_t instructions;

		machine.clear();
		stats.clear();

		llvm::Value * value = nullptr;
		instruction_t * inst = nullptr;

	/*-------------------------------------------------------------------------*
	 * Visit instructions.
	 *-------------------------------------------------------------------------*/

		while(iita != inst_end(fita) || active.size() > 0) {
			size_t issued(0);
			bool issue(true);
			std::vector<instruction_t *> cycle_issue;

			while(iita != inst_end(fita) && issue &&
				issued < core.max_issue()) {
				value = &*iita;

				/*----------------------------------------------------------------*
				 * Create instruction and add dependencies
				 *----------------------------------------------------------------*/

				if(inst == nullptr) {

					inst = new instruction_t(decode(&*iita));

					unsigned operands = iita->getNumOperands();
					for(unsigned i(0); i<operands; ++i) {
						auto op = processed.find(iita->getOperand(i));
						if(op != processed.end()) {
							inst->add_dependency(op->second);
						} // if
					} // for

					/*-------------------------------------------------------------*
					 * If an instruction that was previously issed and stalled
					 * is now ready to execute, we can try for multiple-issue.
					 *-------------------------------------------------------------*/

					if(issued == 0) {
						// Check for pending instructions from previous cycles
						// NOTE: This has to happen before the current instruciton
						// is added to active!
						for(auto a = active.begin(); a != active.end(); ++a) {
							if((*a)->state() == instruction_t::stalled &&
								(*a)->ready()) {
								// reset state to staging
								(*a)->set_state(instruction_t::staging);

								// add to instructions issued this cycle
								cycle_issue.push_back(*a);

								// update count
								++issued;
							} // if
						} // for
					} // if

					/*-------------------------------------------------------------*
					 * Add instruction to active list
					 *-------------------------------------------------------------*/

					active.push_back(inst);
				} // if

				/*-------------------------------------------------------------*
				 * Check for dependencies of this instruction
				 *
				 * If the currently queued instruction has any dependencies
				 * that have not retired, multiple issue cannot happen.
				 *-------------------------------------------------------------*/

				issue = inst->ready();

				/*-------------------------------------------------------------*
				 * Check for dependencies within this issue
				 *
				 * If the current instruction depends on any other instructions
				 * that are to be issued this cycle, multiple issue cannot
				 * happen because the current instruction will immediately
				 * stall.
				 *-------------------------------------------------------------*/

				bool cycle_dependency(false);
				if(issued > 0) {
					for(auto cita = cycle_issue.begin();
						cita != cycle_issue.end(); ++cita) {
						for(auto dita = inst->dependencies().begin();
							dita != inst->dependencies().end(); ++dita) {
								if(*dita == *cita) {
									cycle_dependency = true;
									break;
								} // if
						} // for

						if(cycle_dependency) {
							break;
						} // if
					} // for
				} // if

				/*-------------------------------------------------------------*
				 * Check for stalls
				 *
				 * If any instruction is stalled, no new instructions
				 * can be issued.
				 *-------------------------------------------------------------*/

				bool cycle_stall(false);
				if(!cycle_dependency) {
					for(auto a = active.begin(); /* (*a) != inst && */
						a != active.end(); ++a) {
						// check for any type of stall from an active instruction
						if((*a)->state() == instruction_t::stalled) {
							cycle_stall = true;
							break;
						} // if
					} // for
				} // if

				/*-------------------------------------------------------------*
				 * Check to see if this instruction will stall on issue.
				 *
				 * The previous check will not catch this because the
				 * instruction hasn't yet been advanced.
				 *-------------------------------------------------------------*/

				if(issued > 0 && !issue) {
					cycle_stall = true;
				} // if

				/*-------------------------------------------------------------*
				 * Try to issue
				 *-------------------------------------------------------------*/

				int32_t id = core.accept(inst);
				if(!cycle_dependency && !cycle_stall && id >= 0) {

					/*##############################################################
					 ###############################################################
					 # Everything that makes it to this point actually gets
					 # issued and executed.
					 ###############################################################
					 *#############################################################*/
				
					/*-------------------------------------------------------------*
					 * Update statistics
					 *-------------------------------------------------------------*/

					update_stats(&*iita);
				
					/*-------------------------------------------------------------*
					 * Add instruction to this issue
					 *-------------------------------------------------------------*/
				
					cycle_issue.push_back(inst);

					/*-------------------------------------------------------------*
					 * Add instruction to hash
					 *-------------------------------------------------------------*/

					processed[value] = inst;

					/*-------------------------------------------------------------*
					 * Add instruction to issued instructions
					 *-------------------------------------------------------------*/

					instructions.push_back(inst);

					/*-------------------------------------------------------------*
					 * Advance LLVM instruction stream
					 *-------------------------------------------------------------*/

					++issued;
					++iita;
					inst = nullptr;
				}
				else {
					// cleanup failed multiple issue attempt
					if(cycle_stall || issued > 0) {

						// remove current instruction from active list
						for(auto a = active.begin(); a != active.end(); ++a) {
							if((*a) == inst) {
								active.erase(a);
								break;
							} // if
						} // for

						// free the ALU for the current instruction
						if(id != -1) {
							core.release(id);
						} // if

						// delete the instruction
						delete inst;
						inst = nullptr;
					} // if

					issue = false;
					continue;
				} // if

				// if multiple issue was possible, update the affected
				// instruction states
				if(issued > 1) {
					for(auto cita = cycle_issue.begin();
						cita != cycle_issue.end(); ++cita) {
						(*cita)->set_multiple(issued);
					} // for
				} // if
			} // while

			// update executing instructions
			auto a = active.begin();
			while(a != active.end()) {
				(*a)->advance();

				if((*a)->state() == instruction_t::retired) {
					active.erase(a++);
				}
				else {
					++a;
				} // if
			} // while

			// set state for next cycle
			core.advance();
		} // while

		stream << "KEYWORD_STACK_ALLOCATIONS " <<
			stats["allocas"] << std::endl;
		stream << "KEYWORD_STACK_ALLOCATION_BYTES " <<
			stats["alloca bytes"] << std::endl;
		stream << "KEYWORD_FLOPS " << stats["flops"] << std::endl;
		stream << "KEYWORD_LOADS " << stats["loads"] << std::endl;
		stream << "KEYWORD_LOAD_BYTES " << stats["load bytes"] << std::endl;
		stream << "KEYWORD_STORES " << stats["stores"] << std::endl;
		stream << "KEYWORD_STORE_BYTES " << stats["store bytes"] << std::endl;
		stream << "KEYWORD_CYCLES " << machine.current() << std::endl;

		stream << "BEGIN_INSTRUCTION_STREAM" << std::endl;

		for(auto out = instructions.begin(); out != instructions.end(); ++out) {
			stream << (*out)->string() << std::endl;
		} // for

		stream << "END_INSTRUCTION_STREAM" << std::endl;
		stream << "END_MODULE" << std::endl;
	} // for
} // simulator_t::simulator_t

instruction_properties_t simulator_t::decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	instruction_properties_t properties;

	unsigned opcode = instruction->getOpcode();
	unsigned optype = instruction->getType()->getTypeID();

	std::string str;
	llvm::raw_string_ostream rso(str);
	rso << *instruction;
	properties.ir = rso.str();

	DEBUG(rso.str());

	properties.opcode = opcode;
	properties.optype = optype;

	switch(opcode) {

		/*----------------------------------------------------------------------*
		 * Terminator operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Ret:
			arch.getval(properties.latency, "latency::ret");
			arch.getval(properties.reciprocal, "reciprocal::ret");
			break;

		case llvm::Instruction::Br:
			arch.getval(properties.latency, "latency::br");
			arch.getval(properties.reciprocal, "reciprocal::br");
			break;

		case llvm::Instruction::Switch:
			arch.getval(properties.latency, "latency::switch");
			arch.getval(properties.reciprocal, "reciprocal::switch");
			break;

		case llvm::Instruction::IndirectBr:
			arch.getval(properties.latency, "latency::indirectbr");
			arch.getval(properties.reciprocal, "reciprocal::indirectbr");
			break;

		case llvm::Instruction::Invoke:
			arch.getval(properties.latency, "latency::invoke");
			arch.getval(properties.reciprocal, "reciprocal::invoke");
			break;

		case llvm::Instruction::Resume:
			arch.getval(properties.latency, "latency::resume");
			arch.getval(properties.reciprocal, "reciprocal::resume");
			break;

		case llvm::Instruction::Unreachable:
			arch.getval(properties.latency, "latency::unreachable");
			arch.getval(properties.reciprocal, "reciprocal::unreachable");
			break;

		/*----------------------------------------------------------------------*
		 * Binary operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Add:
			arch.getval(properties.latency, "latency::add");
			arch.getval(properties.reciprocal, "reciprocal::add");
			break;

		case llvm::Instruction::FAdd:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fadd::float");
					arch.getval(properties.reciprocal, "reciprocal::fadd::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fadd::double");
					arch.getval(properties.reciprocal, "reciprocal::fadd::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fadd::vector");
					arch.getval(properties.reciprocal, "reciprocal::fadd::vector");
					break;
				default:
					ExitOnError("FAdd Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Sub:
			arch.getval(properties.latency, "latency::sub");
			arch.getval(properties.reciprocal, "reciprocal::sub");
			break;

		case llvm::Instruction::FSub:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fsub::float");
					arch.getval(properties.reciprocal, "reciprocal::fsub::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fsub::double");
					arch.getval(properties.reciprocal, "reciprocal::fsub::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fsub::vector");
					arch.getval(properties.reciprocal, "reciprocal::fsub::vector");
					break;
				default:
					ExitOnError("FSub Unhandled Type " <<
						instruction->getType()->getTypeID(),
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Mul:
			arch.getval(properties.latency, "latency::mul");
			arch.getval(properties.reciprocal, "reciprocal::mul");
			break;

		case llvm::Instruction::FMul:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fmul::float");
			arch.getval(properties.reciprocal, "reciprocal::fmul::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fmul::double");
			arch.getval(properties.reciprocal, "reciprocal::fmul::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fmul::vector");
			arch.getval(properties.reciprocal, "reciprocal::fmul::vector");
					break;
				default:
					ExitOnError("FMul Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::UDiv:
			arch.getval(properties.latency, "latency::udiv");
			arch.getval(properties.reciprocal, "reciprocal::udiv");
			break;

		case llvm::Instruction::SDiv:
			arch.getval(properties.latency, "latency::sdiv");
			arch.getval(properties.reciprocal, "reciprocal::sdiv");
			break;

		case llvm::Instruction::FDiv:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fdiv::float");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fdiv::double");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fdiv::vector");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::vector");
					break;
				default:
					ExitOnError("FDiv Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::URem:
			arch.getval(properties.latency, "latency::urem");
			arch.getval(properties.reciprocal, "reciprocal::urem");
			break;

		case llvm::Instruction::SRem:
			arch.getval(properties.latency, "latency::srem");
			arch.getval(properties.reciprocal, "reciprocal::srem");
			break;

		case llvm::Instruction::FRem:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::frem::float");
			arch.getval(properties.reciprocal, "reciprocal::frem::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::frem::double");
			arch.getval(properties.reciprocal, "reciprocal::frem::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::frem::vector");
			arch.getval(properties.reciprocal, "reciprocal::frem::vector");
					break;
				default:
					ExitOnError("FRem Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		/*----------------------------------------------------------------------*
		 * Logical operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Shl:
			arch.getval(properties.latency, "latency::shl");
			arch.getval(properties.reciprocal, "reciprocal::shl");
			break;

		case llvm::Instruction::LShr:
			arch.getval(properties.latency, "latency::lshr");
			arch.getval(properties.reciprocal, "reciprocal::lshr");
			break;

		case llvm::Instruction::AShr:
			arch.getval(properties.latency, "latency::ashr");
			arch.getval(properties.reciprocal, "reciprocal::ashr");
			break;

		case llvm::Instruction::And:
			arch.getval(properties.latency, "latency::and");
			arch.getval(properties.reciprocal, "reciprocal::and");
			break;

		case llvm::Instruction::Or:
			arch.getval(properties.latency, "latency::or");
			arch.getval(properties.reciprocal, "reciprocal::or");
			break;

		case llvm::Instruction::Xor:
			arch.getval(properties.latency, "latency::xor");
			arch.getval(properties.reciprocal, "reciprocal::xor");
			break;

		/*----------------------------------------------------------------------*
		 * Memory operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Alloca:
			arch.getval(properties.latency, "latency::alloca");
			arch.getval(properties.reciprocal, "reciprocal::alloca");
			break;

		case llvm::Instruction::Load:
			arch.getval(properties.latency, "latency::load");
			arch.getval(properties.reciprocal, "reciprocal::load");
			break;
	
		case llvm::Instruction::Store:
			arch.getval(properties.latency, "latency::store");
			arch.getval(properties.reciprocal, "reciprocal::store");
			break;
	
		case llvm::Instruction::GetElementPtr:
			arch.getval(properties.latency, "latency::getelementptr");
			arch.getval(properties.reciprocal, "reciprocal::getelementptr");
			break;
	
		case llvm::Instruction::Fence:
			arch.getval(properties.latency, "latency::fence");
			arch.getval(properties.reciprocal, "reciprocal::fence");
			break;
	
		case llvm::Instruction::AtomicCmpXchg:
			arch.getval(properties.latency, "latency::atomiccmpxchg");
			arch.getval(properties.reciprocal, "reciprocal::atomiccmpxchg");
			break;
	
		case llvm::Instruction::AtomicRMW:
			arch.getval(properties.latency, "latency::atomicrmw");
			arch.getval(properties.reciprocal, "reciprocal::atomicrmw");
			break;
	
		/*----------------------------------------------------------------------*
		 * Cast operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Trunc:
			arch.getval(properties.latency, "latency::trunc");
			arch.getval(properties.reciprocal, "reciprocal::trunc");
			break;
	
		case llvm::Instruction::ZExt:
			arch.getval(properties.latency, "latency::zext");
			arch.getval(properties.reciprocal, "reciprocal::zext");
			break;
	
		case llvm::Instruction::SExt:
			arch.getval(properties.latency, "latency::sext");
			arch.getval(properties.reciprocal, "reciprocal::sext");
			break;
	
		case llvm::Instruction::FPToUI:
			arch.getval(properties.latency, "latency::fptoui");
			arch.getval(properties.reciprocal, "reciprocal::fptoui");
			break;
	
		case llvm::Instruction::FPToSI:
			arch.getval(properties.latency, "latency::fptosi");
			arch.getval(properties.reciprocal, "reciprocal::fptosi");
			break;
	
		case llvm::Instruction::UIToFP:
			arch.getval(properties.latency, "latency::uitofp");
			arch.getval(properties.reciprocal, "reciprocal::uitofp");
			break;
	
		case llvm::Instruction::SIToFP:
			arch.getval(properties.latency, "latency::sitofp");
			arch.getval(properties.reciprocal, "reciprocal::sitofp");
			break;
	
		case llvm::Instruction::FPTrunc:
			arch.getval(properties.latency, "latency::fptrunc");
			arch.getval(properties.reciprocal, "reciprocal::fptrunc");
			break;
	
		case llvm::Instruction::FPExt:
			arch.getval(properties.latency, "latency::fpext");
			arch.getval(properties.reciprocal, "reciprocal::fpext");
			break;
	
		case llvm::Instruction::PtrToInt:
			arch.getval(properties.latency, "latency::ptrtoint");
			arch.getval(properties.reciprocal, "reciprocal::ptrtoint");
			break;
	
		case llvm::Instruction::IntToPtr:
			arch.getval(properties.latency, "latency::inttoptr");
			arch.getval(properties.reciprocal, "reciprocal::inttoptr");
			break;
	
		case llvm::Instruction::BitCast:
			arch.getval(properties.latency, "latency::bitcast");
			arch.getval(properties.reciprocal, "reciprocal::bitcast");
			break;
	
		/*----------------------------------------------------------------------*
		 * Other operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::ICmp:
			arch.getval(properties.latency, "latency::icmp");
			arch.getval(properties.reciprocal, "reciprocal::icmp");
			break;

		case llvm::Instruction::FCmp:
			arch.getval(properties.latency, "latency::fcmp");
			arch.getval(properties.reciprocal, "reciprocal::fcmp");
			break;

		case llvm::Instruction::PHI:
			arch.getval(properties.latency, "latency::phi");
			arch.getval(properties.reciprocal, "reciprocal::phi");
			break;

		case llvm::Instruction::Call:
			{
			arch.getval(properties.latency, "latency::call");
			arch.getval(properties.reciprocal, "reciprocal::call");
			llvm::CallInst * cinst = llvm::cast<llvm::CallInst>(instruction);

			std::string call;
			std::string name = cinst->getCalledFunction()->getName().str();

#if defined(USE_MANGLED_CALL_NAMES)
			int status;
			char * um_name = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
			name = um_name;
			delete um_name;
			name = name.substr(0, name.find_first_of("("));
#endif

			// check for x86 intrinsics
			if(x86_match(name)) {
				name = x86_call_map[name];
			} // if

			call = name;
			switch(optype) {
				case llvm::Type::FloatTyID:
					call += "::float";
					break;
				case llvm::Type::DoubleTyID:
					call += "::double";
					break;
				case llvm::Type::VectorTyID:
					call += "::vector";
					break;
				default:
					ExitOnError("Call Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			arch.getval(properties.latency, "latency::" + call);
			arch.getval(properties.reciprocal, "reciprocal::" + call);

			break;
			} // scope
	
		case llvm::Instruction::Select:
			arch.getval(properties.latency, "latency::select");
			arch.getval(properties.reciprocal, "reciprocal::select");
			break;
	
		case llvm::Instruction::UserOp1:
			arch.getval(properties.latency, "latency::userop1");
			arch.getval(properties.reciprocal, "reciprocal::userop1");
			break;
	
		case llvm::Instruction::UserOp2:
			arch.getval(properties.latency, "latency::userop2");
			arch.getval(properties.reciprocal, "reciprocal::userop2");
			break;
	
		case llvm::Instruction::VAArg:
			arch.getval(properties.latency, "latency::vaarg");
			arch.getval(properties.reciprocal, "reciprocal::vaarg");
			break;
	
		case llvm::Instruction::ExtractElement:
			arch.getval(properties.latency, "latency::extractelement");
			arch.getval(properties.reciprocal, "reciprocal::extractelement");
			break;
	
		case llvm::Instruction::InsertElement:
			arch.getval(properties.latency, "latency::insertelement");
			arch.getval(properties.reciprocal, "reciprocal::insertelement");
			break;
	
		case llvm::Instruction::ShuffleVector:
			arch.getval(properties.latency, "latency::shufflevector");
			arch.getval(properties.reciprocal, "reciprocal::shufflevector");
			break;

		case llvm::Instruction::ExtractValue:
			arch.getval(properties.latency, "latency::extractvalue");
			arch.getval(properties.reciprocal, "reciprocal::extractvalue");
			break;
	
		case llvm::Instruction::InsertValue:
			arch.getval(properties.latency, "latency::insertvalue");
			arch.getval(properties.reciprocal, "reciprocal::insertvalue");
			break;
	
		case llvm::Instruction::LandingPad:
			arch.getval(properties.latency, "latency::landingpad");
			arch.getval(properties.reciprocal, "reciprocal::landingpad");
			break;
	
		default:
			ExitOnError("Unhandled Instruction", ska::UnknownCase);

	} // switch

	return properties;
} // simulator_t::decode

void simulator_t::update_stats(llvm::Instruction * instruction) {
	statistics_t & stats = statistics_t::instance();

	switch(instruction->getOpcode()) {
		case llvm::Instruction::Ret:
			break;
		case llvm::Instruction::Br:
			break;
		case llvm::Instruction::Switch:
			break;
		case llvm::Instruction::IndirectBr:
			break;
		case llvm::Instruction::Invoke:
			break;
		case llvm::Instruction::Resume:
			break;
		case llvm::Instruction::Unreachable:
			break;
		case llvm::Instruction::Add:
			break;
		case llvm::Instruction::FAdd:
			stats["flops"]++;
			break;
		case llvm::Instruction::Sub:
			break;
		case llvm::Instruction::FSub:
			stats["flops"]++;
			break;
		case llvm::Instruction::Mul:
			break;
		case llvm::Instruction::FMul:
			stats["flops"]++;
			break;
		case llvm::Instruction::UDiv:
			break;
		case llvm::Instruction::SDiv:
			break;
		case llvm::Instruction::FDiv:
			stats["flops"]++;
			break;
		case llvm::Instruction::URem:
			break;
		case llvm::Instruction::SRem:
			break;
		case llvm::Instruction::FRem:
			stats["flops"]++;
			break;
		case llvm::Instruction::Shl:
		case llvm::Instruction::LShr:
		case llvm::Instruction::AShr:
		case llvm::Instruction::And:
		case llvm::Instruction::Or:
		case llvm::Instruction::Xor:
			break;
		case llvm::Instruction::Alloca:
			{
			stats["allocas"]++;
			llvm::AllocaInst * ainst =
				llvm::cast<llvm::AllocaInst>(instruction);
			stats["alloca bytes"] += bytes(ainst->getType());
			break;
			} // scope
		case llvm::Instruction::Load:
			{
			stats["loads"]++;
			llvm::Value * value = instruction->getOperand(0);
			stats["load bytes"] += bytes(value->getType());
			break;
			} // scope
		case llvm::Instruction::Store:
			{
			stats["stores"]++;
			llvm::Value * value = instruction->getOperand(0);
			stats["store bytes"] += bytes(value->getType());
			break;
			} // scope
		case llvm::Instruction::GetElementPtr:
			break;
		case llvm::Instruction::Fence:
			break;
		case llvm::Instruction::AtomicCmpXchg:
			break;
		case llvm::Instruction::AtomicRMW:
			break;
		case llvm::Instruction::Trunc:
			break;
		case llvm::Instruction::ZExt:
			break;
		case llvm::Instruction::SExt:
			break;
		case llvm::Instruction::FPToUI:
			break;
		case llvm::Instruction::FPToSI:
			break;
		case llvm::Instruction::UIToFP:
			break;
		case llvm::Instruction::SIToFP:
			break;
		case llvm::Instruction::FPTrunc:
			break;
		case llvm::Instruction::FPExt:
			break;
		case llvm::Instruction::PtrToInt:
			break;
		case llvm::Instruction::IntToPtr:
			break;
		case llvm::Instruction::BitCast:
			break;
		case llvm::Instruction::ICmp:
			break;
		case llvm::Instruction::FCmp:
			break;
		case llvm::Instruction::PHI:
			break;
		case llvm::Instruction::Call:
			break;
		case llvm::Instruction::Select:
			break;
		case llvm::Instruction::UserOp1:
			break;
		case llvm::Instruction::UserOp2:
			break;
		case llvm::Instruction::VAArg:
			break;
		case llvm::Instruction::ExtractElement:
			break;
		case llvm::Instruction::InsertElement:
			break;
		case llvm::Instruction::ShuffleVector:
			break;
		case llvm::Instruction::ExtractValue:
			break;
		case llvm::Instruction::InsertValue:
			break;
		case llvm::Instruction::LandingPad:
			break;
		default:
			{
				ExitOnError("Unhandled Instruction", ska::UnknownCase);
			} // scope
	} // switch
} // simulator_t::update_stats

size_t simulator_t::bytes(llvm::Type * type) {
	switch(type->getTypeID()) {

		case llvm::Type::VoidTyID:
			return sizeof(nullptr);

		case llvm::Type::HalfTyID:
			break;

		case llvm::Type::FloatTyID:
			return 4;

		case llvm::Type::DoubleTyID:
			return 8;

		case llvm::Type::X86_FP80TyID:
			break;

		case llvm::Type::FP128TyID:
			break;

		case llvm::Type::PPC_FP128TyID:
			break;

		case llvm::Type::LabelTyID:
			break;

		case llvm::Type::MetadataTyID:
			break;

		case llvm::Type::X86_MMXTyID:
			break;

		case llvm::Type::IntegerTyID:
			break;

		case llvm::Type::FunctionTyID:
			break;

		case llvm::Type::StructTyID:
			break;

		case llvm::Type::ArrayTyID:
			return type->getArrayNumElements() *
				bytes(type->getArrayElementType());

		case llvm::Type::PointerTyID:
			return bytes(type->getPointerElementType());

		case llvm::Type::VectorTyID:
			break;

		case llvm::Type::NumTypeIDs:
			break;

		default:
			break;
	} // switch

	return 0;
} // simulator_t::bytes

} // namespace ska

#endif // Simulator_hh

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

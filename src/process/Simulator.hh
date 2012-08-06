/*----------------------------------------------------------------------------*
 *
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

class simulator_t
{
public:

	typedef std::map<llvm::Value *, instruction_t *> instruction_map_t;
	typedef std::vector<instruction_t *> instruction_vector_t;
	typedef std::list<instruction_t *> instruction_list_t;

	simulator_t(const char * ir_file, std::ostream & stream);
	~simulator_t() {}

	int32_t decode(llvm::Instruction * instruction);
	void update_stats(llvm::Instruction * instruction);

	size_t bytes(llvm::Type * type);

private:

	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

}; // class simulator_t

simulator_t::simulator_t(const char * ir_file, std::ostream & stream)
	: llvm_module_(nullptr)
{
	parameters_t & arch = parameters_t::instance();
	machine_state_t & state = machine_state_t::instance();
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
		DEFINE_TO_STRING(SKA_VERSION) << std::endl << std::endl;
	
	/*-------------------------------------------------------------------------*
	 * Initialize core.
	 *-------------------------------------------------------------------------*/
	
	int32_t max_issue;
	arch.getval(max_issue, "core::max_issue");
	core_t core(max_issue);

	int32_t lus;
	arch.getval(lus, "lus");

	for(int i(0); i<lus; ++i) {
		lu_t * lu = new lu_t(i);

		// parse instructions that this LU can handle
		char key[256];
		std::string tmp;

		sprintf(key, "lu::%d::instructions", i);
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
		sprintf(key, "lu::%d::types", i);
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

	/*-------------------------------------------------------------------------*
	 * Visit modules.
	 *-------------------------------------------------------------------------*/

	for(llvm::Module::iterator mita = llvm_module_->begin();
		mita != llvm_module_->end(); ++mita) {

		llvm::inst_iterator ita = inst_begin(mita);
		
		if(ita == inst_end(mita)) {
			continue;
		} // if

		stream << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		stream << "# Module Section: " << mita->getName().str() << std::endl;
		stream << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		stream << "BEGIN_MODULE" << std::endl;
		stream << "KEYWORD_MODULE_NAME " << mita->getName().str() << std::endl;

		instruction_list_t active;
		instruction_vector_t instructions;

		state.clear();
		stats.clear();

		llvm::Value * value = nullptr;
		instruction_t * inst = nullptr;

	/*-------------------------------------------------------------------------*
	 * Visit instructions.
	 *-------------------------------------------------------------------------*/

		while(ita != inst_end(mita) || active.size()) {
			size_t issued(0);
			bool issue(true);
			std::vector<instruction_t *> cycle_issue;

			while(ita != inst_end(mita) && issue &&
				issued < core.max_issue()) {

				// get instruction latency
				int32_t latency = decode(&*ita);

				std::string str;
				llvm::raw_string_ostream rso(str);
				rso << *ita;
				value = &*ita;			

				DEBUG(rso.str());

				/*----------------------------------------------------------------*
				 * Create instruction and add dependencies
				 *----------------------------------------------------------------*/

				inst = new instruction_t(latency, ita->getOpcode(),
					ita->getType()->getTypeID(), rso.str());

				unsigned operands = ita->getNumOperands();
				for(unsigned i(0); i<operands; ++i) {
					auto op = processed.find(ita->getOperand(i));
					if(op != processed.end()) {
						inst->add_dependency(op->second);
					} // if
				} // for

				int32_t id = core.accept(inst);
				if(id >= 0) {

					// currently, this means that the opcode was not recognized
					if(latency == -1) {
						++ita;
						issue = false;
						continue;
					} // if

					/*-------------------------------------------------------------*
					 * Check for dependencies within this issue
					 *-------------------------------------------------------------*/

					for(auto cita = cycle_issue.begin();
						cita != cycle_issue.end(); ++cita) {
						for(auto dita = inst->dependencies().begin();
							dita != inst->dependencies().end(); ++dita) {
								if(*dita == *cita) {
									issue = false;
								} // if
						} // for
					} // for

					if(issue == false) {
						core.release(id);
						delete inst;
						inst = nullptr;
						continue;
					} // if
			
					/*##############################################################
					 ###############################################################
					 # Everything that makes it to this point actually gets
					 # issued and executed.
					 ###############################################################
					 *#############################################################*/
				
					/*-------------------------------------------------------------*
					 * Update statistics
					 *-------------------------------------------------------------*/

					update_stats(&*ita);
				
					/*-------------------------------------------------------------*
					 * Add instruction to this issue
					 *-------------------------------------------------------------*/
				
					cycle_issue.push_back(inst);

					/*-------------------------------------------------------------*
					 * Add instruction to active list
					 *-------------------------------------------------------------*/

					active.push_back(inst);

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
					++ita;
				}
				else {
					delete inst;
					inst = nullptr;
					issue = false;
					continue;
				} // if

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
			} // for

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
		stream << "KEYWORD_CYCLES " << state.current() << std::endl;

		stream << "BEGIN_INSTRUCTION_STREAM" << std::endl;

		for(auto ita = instructions.begin(); ita != instructions.end(); ++ita) {
			stream << (*ita)->string() << std::endl;
		} // for

		stream << "END_INSTRUCTION_STREAM" << std::endl;
		stream << "END_MODULE" << std::endl;
	} // for
} // simulator_t::simulator_t

int32_t simulator_t::decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	int32_t latency(-1);

	switch(instruction->getOpcode()) {

		/*----------------------------------------------------------------------*
		 * Terminator operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Ret:
			arch.getval(latency, "latency::ret");
			break;

		case llvm::Instruction::Br:
			arch.getval(latency, "latency::br");
			break;

		case llvm::Instruction::Switch:
			arch.getval(latency, "latency::switch");
			break;

		case llvm::Instruction::IndirectBr:
			arch.getval(latency, "latency::indirectbr");
			break;

		case llvm::Instruction::Invoke:
			arch.getval(latency, "latency::invoke");
			break;

		case llvm::Instruction::Resume:
			arch.getval(latency, "latency::resume");
			break;

		case llvm::Instruction::Unreachable:
			arch.getval(latency, "latency::unreachable");
			break;

		/*----------------------------------------------------------------------*
		 * Binary operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Add:
			arch.getval(latency, "latency::add");
			break;

		case llvm::Instruction::FAdd:
			// get the instruction latency
			switch(instruction->getType()->getTypeID()) {
				case llvm::Type::FloatTyID:
					arch.getval(latency, "latency::fadd::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(latency, "latency::fadd::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(latency, "latency::fadd::vector");
					break;
				default:
					ExitOnError("FAdd Unhandled Type",
						ErrCode::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Sub:
			arch.getval(latency, "latency::sub");
			break;

		case llvm::Instruction::FSub:
			// get the instruction latency
			switch(instruction->getType()->getTypeID()) {
				case llvm::Type::FloatTyID:
					arch.getval(latency, "latency::fsub::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(latency, "latency::fsub::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(latency, "latency::fsub::vector");
					break;
				default:
					ExitOnError("FSub Unhandled Type " <<
						instruction->getType()->getTypeID(),
						ErrCode::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Mul:
			arch.getval(latency, "latency::mul");
			break;

		case llvm::Instruction::FMul:
			// get the instruction latency
			switch(instruction->getType()->getTypeID()) {
				case llvm::Type::FloatTyID:
					arch.getval(latency, "latency::fmul::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(latency, "latency::fmul::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(latency, "latency::fmul::vector");
					break;
				default:
					ExitOnError("FMul Unhandled Type",
						ErrCode::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::UDiv:
			arch.getval(latency, "latency::udiv");
			break;

		case llvm::Instruction::SDiv:
			arch.getval(latency, "latency::sdiv");
			break;

		case llvm::Instruction::FDiv:
			// get the instruction latency
			switch(instruction->getType()->getTypeID()) {
				case llvm::Type::FloatTyID:
					arch.getval(latency, "latency::fdiv::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(latency, "latency::fdiv::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(latency, "latency::fdiv::vector");
					break;
				default:
					ExitOnError("FDiv Unhandled Type",
						ErrCode::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::URem:
			arch.getval(latency, "latency::urem");
			break;

		case llvm::Instruction::SRem:
			arch.getval(latency, "latency::srem");
			break;

		case llvm::Instruction::FRem:
			// get the instruction latency
			switch(instruction->getType()->getTypeID()) {
				case llvm::Type::FloatTyID:
					arch.getval(latency, "latency::frem::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(latency, "latency::frem::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(latency, "latency::frem::vector");
					break;
				default:
					ExitOnError("FRem Unhandled Type",
						ErrCode::UnknownCase);
					break;
			} // switch

			break;

		/*----------------------------------------------------------------------*
		 * Logical operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Shl:
			arch.getval(latency, "latency::shl");
			break;

		case llvm::Instruction::LShr:
			arch.getval(latency, "latency::lshr");
			break;

		case llvm::Instruction::AShr:
			arch.getval(latency, "latency::ashr");
			break;

		case llvm::Instruction::And:
			arch.getval(latency, "latency::and");
			break;

		case llvm::Instruction::Or:
			arch.getval(latency, "latency::or");
			break;

		case llvm::Instruction::Xor:
			arch.getval(latency, "latency::xor");
			break;

		/*----------------------------------------------------------------------*
		 * Memory operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Alloca:
			arch.getval(latency, "latency::alloca");
			break;

		case llvm::Instruction::Load:
			arch.getval(latency, "latency::load");
			break;
	
		case llvm::Instruction::Store:
			arch.getval(latency, "latency::store");
			break;
	
		case llvm::Instruction::GetElementPtr:
			arch.getval(latency, "latency::getelementptr");
			break;
	
		case llvm::Instruction::Fence:
			arch.getval(latency, "latency::fence");
			break;
	
		case llvm::Instruction::AtomicCmpXchg:
			arch.getval(latency, "latency::atomiccmpxchg");
			break;
	
		case llvm::Instruction::AtomicRMW:
			arch.getval(latency, "latency::atomicrmw");
			break;
	
		/*----------------------------------------------------------------------*
		 * Cast operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Trunc:
			arch.getval(latency, "latency::trunc");
			break;
	
		case llvm::Instruction::ZExt:
			arch.getval(latency, "latency::zext");
			break;
	
		case llvm::Instruction::SExt:
			arch.getval(latency, "latency::sext");
			break;
	
		case llvm::Instruction::FPToUI:
			arch.getval(latency, "latency::fptoui");
			break;
	
		case llvm::Instruction::FPToSI:
			arch.getval(latency, "latency::fptosi");
			break;
	
		case llvm::Instruction::UIToFP:
			arch.getval(latency, "latency::uitofp");
			break;
	
		case llvm::Instruction::SIToFP:
			arch.getval(latency, "latency::sitofp");
			break;
	
		case llvm::Instruction::FPTrunc:
			arch.getval(latency, "latency::fptrunc");
			break;
	
		case llvm::Instruction::FPExt:
			arch.getval(latency, "latency::fpext");
			break;
	
		case llvm::Instruction::PtrToInt:
			arch.getval(latency, "latency::ptrtoint");
			break;
	
		case llvm::Instruction::IntToPtr:
			arch.getval(latency, "latency::inttoptr");
			break;
	
		case llvm::Instruction::BitCast:
			arch.getval(latency, "latency::bitcast");
			break;
	
		/*----------------------------------------------------------------------*
		 * Other operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::ICmp:
			arch.getval(latency, "latency::icmp");
			break;

		case llvm::Instruction::FCmp:
			arch.getval(latency, "latency::fcmp");
			break;

		case llvm::Instruction::PHI:
			arch.getval(latency, "latency::phi");
			break;

		case llvm::Instruction::Call:
			{
			arch.getval(latency, "latency::call");
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

			call = "latency::" + name;

			switch(instruction->getType()->getTypeID()) {
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
						ErrCode::UnknownCase);
					break;
			} // switch

			arch.getval(latency, call);

			break;
			} // scope
	
		case llvm::Instruction::Select:
			arch.getval(latency, "latency::select");
			break;
	
		case llvm::Instruction::UserOp1:
			arch.getval(latency, "latency::userop1");
			break;
	
		case llvm::Instruction::UserOp2:
			arch.getval(latency, "latency::userop2");
			break;
	
		case llvm::Instruction::VAArg:
			arch.getval(latency, "latency::vaarg");
			break;
	
		case llvm::Instruction::ExtractElement:
			arch.getval(latency, "latency::extractelement");
			break;
	
		case llvm::Instruction::InsertElement:
			arch.getval(latency, "latency::insertelement");
			break;
	
		case llvm::Instruction::ShuffleVector:
			arch.getval(latency, "latency::shufflevector");
			break;

		case llvm::Instruction::ExtractValue:
			arch.getval(latency, "latency::extractvalue");
			break;
	
		case llvm::Instruction::InsertValue:
			arch.getval(latency, "latency::insertvalue");
			break;
	
		case llvm::Instruction::LandingPad:
			arch.getval(latency, "latency::landingpad");
			break;
	
		default:
			ExitOnError("Unhandled Instruction", ErrCode::UnknownCase);

	} // switch

	return latency;
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
				ExitOnError("Unhandled Instruction", ErrCode::UnknownCase);
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

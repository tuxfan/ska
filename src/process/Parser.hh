/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#ifndef Parser_hh
#define Parser_hh

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
#include <Core.hh>

namespace ska {

class parser_t
{
public:

	typedef std::map<llvm::Value *, instruction_t *> instruction_map_t;
	typedef std::vector<instruction_t *> instruction_vector_t;
	typedef std::list<instruction_t *> instruction_list_t;

	parser_t(const char * ir_file);
	~parser_t() {}

	int32_t decode(llvm::Instruction * instruction);

	size_t alloca_bytes(llvm::Type * type);

private:

	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

}; // class parser_t

parser_t::parser_t(const char * ir_file)
	: llvm_module_(nullptr)
{
	parameters_t & arch = parameters_t::instance();
	machine_state_t & state = machine_state_t::instance();
	statistics_t & stats = statistics_t::instance();
	instruction_map_t processed;

//////
// Initialize core
	
	int32_t max_issue;
	arch.getval(max_issue, "core::max_issue");
	core_t core(max_issue);

	int32_t alus;
	arch.getval(alus, "alus");

	for(int i(0); i<alus; ++i) {
		alu_t * alu = new alu_t(i);

		char key[256];
		sprintf(key, "alu::%d", i);

		std::string tmp;
		arch.getval(tmp, key);
		char * ops = new char[tmp.size()+1];
		strcpy(ops, tmp.c_str());

		char * tok = strtok(ops, " ");

		while(tok != NULL) {
			alu->add_op(code_map[tok]);
			tok = strtok(NULL, " ");
		} // while

		core.add_unit(alu);

		delete[] ops;
	} // for

////////

	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	// visit modules
	for(llvm::Module::iterator mita = llvm_module_->begin();
		mita != llvm_module_->end(); ++mita) {

		llvm::inst_iterator ita = inst_begin(mita);
		
		instruction_list_t active;
		instruction_vector_t instructions;

		state.reset();

		llvm::Value * value = nullptr;
		instruction_t * inst = nullptr;

		while(ita != inst_end(mita) || active.size()) {

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

			size_t issued(0);
			bool issue(true);
			std::vector<instruction_t *> cycle_issue;
			while(ita != inst_end(mita) && issue && issued < core.max_issue()) {

				// get instruction latency
				int32_t latency = decode(&*ita);

				std::string str;
				llvm::raw_string_ostream rso(str);
				rso << *ita;
				value = &*ita;			

				/*----------------------------------------------------------------*
				 * Create instruction and add dependencies
				 *----------------------------------------------------------------*/

				inst = new instruction_t(latency, rso.str());

				unsigned operands = ita->getNumOperands();
				for(unsigned i(0); i<operands; ++i) {
					auto op = processed.find(ita->getOperand(i));
					if(op != processed.end()) {
						inst->add_dependency(op->second);
					} // if
				} // for

				int32_t id = core.accept(ita->getOpcode(), inst);
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
						delete inst;
						inst = nullptr;
						continue;
					} // if
			
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
			} // while
			
			core.advance();
		} // while

		for(auto ita = instructions.begin(); ita != instructions.end(); ++ita) {
			std::cout << (*ita)->string() << std::endl;
		} // for

		std::cout << std::endl;
		std::cout << "allocs: " << stats["allocs"] << std::endl;
		std::cout << "flops: " << stats["flops"] << std::endl;
		std::cout << "loads: " << stats["loads"] << std::endl;
		std::cout << "stores: " << stats["stores"] << std::endl;
	} // for
} // parser_t::parser_t

int32_t parser_t::decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	statistics_t & stats = statistics_t::instance();
	int32_t latency(-1);

	switch(instruction->getOpcode()) {

		/*----------------------------------------------------------------------*
		 * Terminator operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Ret:
			{
				arch.getval(latency, "latency::ret");
				break;
			} // scope

		case llvm::Instruction::Br:
			{
				arch.getval(latency, "latency::br");
				break;
			} // scope

		case llvm::Instruction::Switch:
			{
				arch.getval(latency, "latency::switch");
				break;
			} // scope

		case llvm::Instruction::IndirectBr:
			{
				arch.getval(latency, "latency::indirectbr");
				break;
			} // scope

		case llvm::Instruction::Invoke:
			{
				arch.getval(latency, "latency::invoke");
				break;
			} // scope

		case llvm::Instruction::Resume:
			{
				arch.getval(latency, "latency::resume");
				break;
			} // scope

		case llvm::Instruction::Unreachable:
			{
				arch.getval(latency, "latency::unreachable");
				break;
			} // scope

		/*----------------------------------------------------------------------*
		 * Binary operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Add:
			{
				arch.getval(latency, "latency::add");
				break;
			} // scope

		case llvm::Instruction::FAdd:
			{
				// get the instruction latency
				switch(instruction->getType()->getTypeID()) {
					case llvm::Type::FloatTyID:
						arch.getval(latency, "latency::fadd::float");
						break;
					case llvm::Type::DoubleTyID:
						arch.getval(latency, "latency::fadd::double");
						break;
					default:
						ExitOnError("FAdd Unhandled Type",
							ErrCode::UnknownCase);
						break;
				} // switch

				// add flop to stats tracker
				stats["flops"]++;
				break;
			} // scope

		case llvm::Instruction::Sub:
			{
				arch.getval(latency, "latency::sub");
				break;
			} // scope

		case llvm::Instruction::FSub:
			{
				// get the instruction latency
				switch(instruction->getType()->getTypeID()) {
					case llvm::Type::FloatTyID:
						arch.getval(latency, "latency::fsub::float");
						break;
					case llvm::Type::DoubleTyID:
						arch.getval(latency, "latency::fsub::double");
						break;
					default:
						ExitOnError("FSub Unhandled Type",
							ErrCode::UnknownCase);
						break;
				} // switch

				// add flop to stats tracker
				stats["flops"]++;
				break;
			} // scope

		case llvm::Instruction::Mul:
			{
				arch.getval(latency, "latency::mul");
				break;
			} // scope

		case llvm::Instruction::FMul:
			{
				// get the instruction latency
				switch(instruction->getType()->getTypeID()) {
					case llvm::Type::FloatTyID:
						arch.getval(latency, "latency::fmul::float");
						break;
					case llvm::Type::DoubleTyID:
						arch.getval(latency, "latency::fmul::double");
						break;
					default:
						ExitOnError("FMul Unhandled Type",
							ErrCode::UnknownCase);
						break;
				} // switch

				// add flop to stats tracker
				stats["flops"]++;
				break;
			} // scope

		case llvm::Instruction::UDiv:
			{
				arch.getval(latency, "latency::udiv");
				break;
			} // scope

		case llvm::Instruction::SDiv:
			{
				arch.getval(latency, "latency::sdiv");
				break;
			} // scope

		case llvm::Instruction::FDiv:
			{
				// get the instruction latency
				switch(instruction->getType()->getTypeID()) {
					case llvm::Type::FloatTyID:
						arch.getval(latency, "latency::fdiv::float");
						break;
					case llvm::Type::DoubleTyID:
						arch.getval(latency, "latency::fdiv::double");
						break;
					default:
						ExitOnError("FDiv Unhandled Type",
							ErrCode::UnknownCase);
						break;
				} // switch

				// add flop to stats tracker
				stats["flops"]++;
				break;
			} // scope

		case llvm::Instruction::URem:
			{
				arch.getval(latency, "latency::urem");
				break;
			} // scope

		case llvm::Instruction::SRem:
			{
				arch.getval(latency, "latency::srem");
				break;
			} // scope

		case llvm::Instruction::FRem:
			{
				// get the instruction latency
				switch(instruction->getType()->getTypeID()) {
					case llvm::Type::FloatTyID:
						arch.getval(latency, "latency::frem::float");
						break;
					case llvm::Type::DoubleTyID:
						arch.getval(latency, "latency::frem::double");
						break;
					default:
						ExitOnError("FRem Unhandled Type",
							ErrCode::UnknownCase);
						break;
				} // switch

				// add flop to stats tracker
				stats["flops"]++;
				break;
			} // scope

		/*----------------------------------------------------------------------*
		 * Logical operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Shl:
			{
				arch.getval(latency, "latency::shl");
				break;
			} // scope

		case llvm::Instruction::LShr:
			{
				arch.getval(latency, "latency::lshr");
				break;
			} // scope

		case llvm::Instruction::AShr:
			{
				arch.getval(latency, "latency::ashr");
				break;
			} // scope

		case llvm::Instruction::And:
			{
				arch.getval(latency, "latency::and");
				break;
			} // scope

		case llvm::Instruction::Or:
			{
				arch.getval(latency, "latency::or");
				break;
			} // scope

		case llvm::Instruction::Xor:
			{
				arch.getval(latency, "latency::xor");
				break;
			} // scope

		/*----------------------------------------------------------------------*
		 * Memory operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Alloca:
			{
				// get the instruction latency
				arch.getval(latency, "latency::alloca");

				// get the amount of stack memory allocated
				llvm::AllocaInst * ainst =
					llvm::cast<llvm::AllocaInst>(instruction);
				stats["allocs"] += alloca_bytes(ainst->getType());

				break;
			} // scope

		case llvm::Instruction::Load:
			{
				// get the instruction latency
				arch.getval(latency, "latency::load");

				// add load to stats tracker
				stats["loads"]++;
				break;
			} // scope
	
		case llvm::Instruction::Store:
			{
				// get the instruction latency
				arch.getval(latency, "latency::store");

				// add load to stats tracker
				stats["stores"]++;
				break;
			} // scope
	
		case llvm::Instruction::GetElementPtr:
			{
				// get the instruction latency
				arch.getval(latency, "latency::getelementptr");
				break;
			} // scope
	
		case llvm::Instruction::Fence:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fence");
				break;
			} // scope
	
		case llvm::Instruction::AtomicCmpXchg:
			{
				// get the instruction latency
				arch.getval(latency, "latency::atomiccmpxchg");
				break;
			} // scope
	
		case llvm::Instruction::AtomicRMW:
			{
				// get the instruction latency
				arch.getval(latency, "latency::atomicrmw");
				break;
			} // scope
	
		/*----------------------------------------------------------------------*
		 * Cast operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Trunc:
			{
				// get the instruction latency
				arch.getval(latency, "latency::trunc");
				break;
			} // scope
	
		case llvm::Instruction::ZExt:
			{
				// get the instruction latency
				arch.getval(latency, "latency::zext");
				break;
			} // scope
	
		case llvm::Instruction::SExt:
			{
				// get the instruction latency
				arch.getval(latency, "latency::sext");
				break;
			} // scope
	
		case llvm::Instruction::FPToUI:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fptoui");
				break;
			} // scope
	
		case llvm::Instruction::FPToSI:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fptosi");
				break;
			} // scope
	
		case llvm::Instruction::UIToFP:
			{
				// get the instruction latency
				arch.getval(latency, "latency::uitofp");
				break;
			} // scope
	
		case llvm::Instruction::SIToFP:
			{
				// get the instruction latency
				arch.getval(latency, "latency::sitofp");
				break;
			} // scope
	
		case llvm::Instruction::FPTrunc:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fptrunc");
				break;
			} // scope
	
		case llvm::Instruction::FPExt:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fpext");
				break;
			} // scope
	
		case llvm::Instruction::PtrToInt:
			{
				// get the instruction latency
				arch.getval(latency, "latency::ptrtoint");
				break;
			} // scope
	
		case llvm::Instruction::IntToPtr:
			{
				// get the instruction latency
				arch.getval(latency, "latency::inttoptr");
				break;
			} // scope
	
		case llvm::Instruction::BitCast:
			{
				// get the instruction latency
				arch.getval(latency, "latency::bitcast");
				break;
			} // scope
	
		/*----------------------------------------------------------------------*
		 * Other operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::ICmp:
			{
				// get the instruction latency
				arch.getval(latency, "latency::icmp");
				break;
			} // scope

		case llvm::Instruction::FCmp:
			{
				// get the instruction latency
				arch.getval(latency, "latency::fcmp");
				break;
			} // scope

		case llvm::Instruction::PHI:
			{
				// get the instruction latency
				arch.getval(latency, "latency::phi");
				break;
			} // scope

		case llvm::Instruction::Call:
			{
				// get the instruction latency
				arch.getval(latency, "latency::call");
				break;
			} // scope
	
		case llvm::Instruction::Select:
			{
				// get the instruction latency
				arch.getval(latency, "latency::select");
				break;
			} // scope
	
		case llvm::Instruction::UserOp1:
			{
				// get the instruction latency
				arch.getval(latency, "latency::userop1");
				break;
			} // scope
	
		case llvm::Instruction::UserOp2:
			{
				// get the instruction latency
				arch.getval(latency, "latency::userop2");
				break;
			} // scope
	
		case llvm::Instruction::VAArg:
			{
				// get the instruction latency
				arch.getval(latency, "latency::vaarg");
				break;
			} // scope
	
		case llvm::Instruction::ExtractElement:
			{
				// get the instruction latency
				arch.getval(latency, "latency::extractelement");
				break;
			} // scope
	
		case llvm::Instruction::InsertElement:
			{
				// get the instruction latency
				arch.getval(latency, "latency::insertelement");
				break;
			} // scope
	
		case llvm::Instruction::ShuffleVector:
			{
				// get the instruction latency
				arch.getval(latency, "latency::shufflevector");
				break;
			} // scope

		case llvm::Instruction::ExtractValue:
			{
				// get the instruction latency
				arch.getval(latency, "latency::extractvalue");
				break;
			} // scope
	
		case llvm::Instruction::InsertValue:
			{
				// get the instruction latency
				arch.getval(latency, "latency::insertvalue");
				break;
			} // scope
	
		case llvm::Instruction::LandingPad:
			{
				// get the instruction latency
				arch.getval(latency, "latency::landingpad");
				break;
			} // scope
	
		default:
			{
				ExitOnError("Unhandled Instruction", ErrCode::UnknownCase);
			} // scope
	} // switch

	return latency;
} // parser_t::decode

size_t parser_t::alloca_bytes(llvm::Type * type) {
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
				alloca_bytes(type->getArrayElementType());

		case llvm::Type::PointerTyID:
			return alloca_bytes(type->getPointerElementType());

		case llvm::Type::VectorTyID:
			break;

		case llvm::Type::NumTypeIDs:
			break;

		default:
			break;
	} // switch

	return 0;
} // parser_t::alloca_bytes

} // namespace ska

#endif // Parser_hh

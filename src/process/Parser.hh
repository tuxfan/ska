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
			while(issue && issued < core.max_issue()) {
				int32_t id = core.accept(ita->getOpcode());
				if(id >= 0) {
					value = &*ita;			

					std::string str;
					llvm::raw_string_ostream rso(str);
					rso << *ita;

					int32_t latency = decode(&*ita);

					// currently, this means that the opcode was not recognized
					if(latency == -1) {
						++ita;
						issue = false;
						continue;
					} // if

					/*-------------------------------------------------------------*
					 * Create instruction and add dependencies
					 *-------------------------------------------------------------*/

					inst = new instruction_t(id, latency, issued > 0, rso.str());

					unsigned operands = ita->getNumOperands();
					for(unsigned i(0); i<operands; ++i) {
						auto op = processed.find(ita->getOperand(i));
						if(op != processed.end()) {
							inst->add_dependency(op->second);
						} // if
					} // for
			
					/*-------------------------------------------------------------*
					 * Add instruction to active list
					 *-------------------------------------------------------------*/

					active.push_back(inst);

					/*-------------------------------------------------------------*
					 * Add instruction to hash
					 *-------------------------------------------------------------*/

					processed[value] = inst;

					instructions.push_back(inst);

					/*-------------------------------------------------------------*
					 * Advance LLVM instruction stream
					 *-------------------------------------------------------------*/

					++issued;
					++ita;
				}
				else {
					issue = false;
					continue;
				} // if
			} // while
			
			// FIXME: consolidate
			core.advance();
			state.advance();
		} // while

		for(auto ita = instructions.begin(); ita != instructions.end(); ++ita) {
			std::cerr << (*ita)->string() << std::endl;
		} // for

		std::cerr << "allocs: " << stats["allocs"] << std::endl;
		std::cerr << "flops: " << stats["flops"] << std::endl;
		std::cerr << "loads: " << stats["loads"] << std::endl;
	} // for
} // parser_t::parser_t

int32_t parser_t::decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	statistics_t & stats = statistics_t::instance();
	int32_t latency(-1);

	switch(instruction->getOpcode()) {

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
	
		case llvm::Instruction::Br:
			{
				// get the instruction latency
				arch.getval(latency, "latency::br");
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
				arch.getval(latency, "latency::phi");
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

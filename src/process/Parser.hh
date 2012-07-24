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
#include <llvm/Function.h>
#include <llvm/Instruction.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Support/InstIterator.h>

#include <Instruction.hh>
#include <MachineState.hh>
#include <Metric.hh>
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
	metric_t & metric = metric_t::instance();
	instruction_map_t processed;

//////
// Initialize core
	
	int32_t max_issue;
	arch.getval(max_issue, "core::max_issue");
	core_t core(max_issue);

	int32_t alus;
	arch.getval(alus, "alus");

	for(int i(0); i<alus; ++i) {
		alu_t * alu = new alu_t;

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
				if(core.accept(ita->getOpcode())) {
					value = &*ita;			

					std::string str;
					llvm::raw_string_ostream rso(str);
					rso << *ita;

					std::cerr << rso.str() << std::endl;

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

					inst = new instruction_t(latency, rso.str());

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
#if 0			
			//
			llvm::Value * value = &*ita;
			instruction_t * inst = nullptr;

			/*-------------------------------------------------------------------*
			 * Get string version of LLVM Value
			 *-------------------------------------------------------------------*/

			std::string str;
			llvm::raw_string_ostream rso(str);
			rso << *ita;

			/*-------------------------------------------------------------------*
			 * Create instruction
			 *-------------------------------------------------------------------*/

			int32_t latency = decode(&*ita);

			if(latency == -1) {
				++ita;
				state.advance();
				continue;
			} // if

			inst = new instruction_t(latency, rso.str());

			// FIXME
			state.advance();

			/*-------------------------------------------------------------------*
			 * Add dependencies to instruction
			 *-------------------------------------------------------------------*/

			unsigned operands = ita->getNumOperands();
			for(unsigned i(0); i<operands; ++i) {
				auto op = processed.find(ita->getOperand(i));
				if(op != processed.end()) {
					inst->add_dependency(op->second);
				} // if
			} // for
			
			// need issue logic
			active.push_back(inst);

			instructions.push_back(inst);

			processed[value] = inst;
			++ita;
#endif
		} // while

		for(auto ita = instructions.begin(); ita != instructions.end(); ++ita) {
			std::cerr << (*ita)->string() << std::endl;
		} // for

		std::cerr << "flops: " << metric["flops"] << std::endl;
		std::cerr << "loads: " << metric["loads"] << std::endl;
	} // for
} // parser_t::parser_t

int32_t parser_t::decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	metric_t & metric = metric_t::instance();
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

				// add flop to metric tracker
				metric["flops"]++;
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

				// add flop to metric tracker
				metric["flops"]++;
				break;
			} // scope

		case llvm::Instruction::Alloca:
			{
				// get the instruction latency
				arch.getval(latency, "latency::alloca");

				// FIXME: add bytes allocated

				break;
			} // scope

		case llvm::Instruction::Load:
			{
				// get the instruction latency
				arch.getval(latency, "latency::load");

				// add load to metric tracker
				metric["loads"]++;
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
	
		// FIXME
		default:
			{
#if 0
				++ita;
				state.advance();
				continue;
				//ExitOnError("Unhandled Instruction", ErrCode::UnknownCase);
#endif
				break;
			} // scope
	} // switch

	return latency;
} // parser_t::decode

} // namespace ska

#endif // Parser_hh

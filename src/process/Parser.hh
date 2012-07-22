/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#ifndef Parser_hh
#define Parser_hh

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

namespace atlas {

class parser_t
{
public:

	typedef std::map<llvm::Value *, instruction_t *> instruction_map_t;
	typedef std::vector<instruction_t *> instruction_vector_t;
	typedef std::list<instruction_t *> instruction_list_t;

	parser_t(const char * ir_file);
	~parser_t() {}

private:

	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

}; // class parser_t

parser_t::parser_t(const char * ir_file)
	: llvm_module_(nullptr)
{
	parameters_t & p = parameters_t::instance();
	machine_state_t & state = machine_state_t::instance();
	metric_t & metric = metric_t::instance();
	instruction_map_t processed;

	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	// visit modules
	for(llvm::Module::iterator mita = llvm_module_->begin();
		mita != llvm_module_->end(); ++mita) {

		llvm::inst_iterator ita = inst_begin(mita);
		
		int32_t int_val;
		instruction_list_t active;
		instruction_vector_t function;

		state.reset();

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

			// issue new instructions
			llvm::Value * value = &*ita;
			instruction_t * inst = nullptr;

			// get instruction information
			std::string str;
			llvm::raw_string_ostream rso(str);
			rso << *ita;

			switch(ita->getOpcode()) {

				case llvm::Instruction::FAdd:
					switch(ita->getType()->getTypeID()) {
						case llvm::Type::FloatTyID:
							p.getval(int_val, "latency::fadd::float");
							break;
						case llvm::Type::DoubleTyID:
							p.getval(int_val, "latency::fadd::double");
							break;
						default:
							ExitOnError("FAdd Unhandled Type",
								ErrCode::UnknownCase);
							break;
					} // switch

					metric["flops"]++;

					std::cerr << "Found fadd" << std::endl;
					inst = new instruction_t(int_val, rso.str());
					break;

				case llvm::Instruction::FMul:
					switch(ita->getType()->getTypeID()) {
						case llvm::Type::FloatTyID:
							p.getval(int_val, "latency::fmul::float");
							break;
						case llvm::Type::DoubleTyID:
							p.getval(int_val, "latency::fmul::double");
							break;
						default:
							ExitOnError("FMul Unhandled Type",
								ErrCode::UnknownCase);
							break;
					} // switch

					metric["flops"]++;

					std::cerr << "Found fmul" << std::endl;
					inst = new instruction_t(int_val, rso.str());
					break;

				case llvm::Instruction::Alloca:
					std::cerr << "Found alloca" << std::endl;
					p.getval(int_val, "latency::alloca");
					inst = new instruction_t(int_val, rso.str());
					break;

				case llvm::Instruction::Load:
					std::cerr << "Found load" << std::endl;

					metric["loads"]++;

					p.getval(int_val, "latency::load");
					inst = new instruction_t(int_val, rso.str());
					break;
			
				case llvm::Instruction::GetElementPtr:
					std::cerr << "Found getelementptr" << std::endl;
					p.getval(int_val, "latency::getelementptr");
					inst = new instruction_t(int_val, rso.str());
					break;
			
				case llvm::Instruction::Br:
					std::cerr << "Found br" << std::endl;
					p.getval(int_val, "latency::br");
					inst = new instruction_t(int_val, rso.str());
					break;
			
				case llvm::Instruction::PHI:
					std::cerr << "Found phi" << std::endl;
					p.getval(int_val, "latency::phi");
					inst = new instruction_t(int_val, rso.str());
					break;
			
				// FIXME
				default:
					++ita;
					state.advance();
					continue;
					//ExitOnError("Unhandled Instruction", ErrCode::UnknownCase);
					break;
			} // switch

			// FIXME
			state.advance();

			// add dependencies
			unsigned operands = ita->getNumOperands();
			for(unsigned i(0); i<operands; ++i) {
				auto op = processed.find(ita->getOperand(i));
				if(op != processed.end()) {
					inst->add_dependency(op->second);
				} // if
			} // for
			
			// need issue logic
			active.push_back(inst);

			function.push_back(inst);

			processed[value] = inst;
			++ita;
		} // while

		for(auto ita = function.begin(); ita != function.end(); ++ita) {
			std::cerr << (*ita)->string() << std::endl;
		} // for

		std::cerr << "flops: " << metric["flops"] << std::endl;
		std::cerr << "loads: " << metric["loads"] << std::endl;
	} // for
} // parser_t::parser_t

} // namespace atlas

#endif // Parser_hh

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
	instruction_map_t processed;

	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	// visit modules
	for(llvm::Module::iterator mita = llvm_module_->begin();
		mita != llvm_module_->end(); ++mita) {

		llvm::inst_iterator ita = inst_begin(mita);

		
		int32_t int_val;
		machine_state_t machine;
		instruction_list_t active;

		while(ita != inst_end(mita)) {

			// update queued instructions
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
			instruction_t * inst;

			switch(ita->getOpcode()) {

				case llvm::Instruction::FAdd:
					switch(ita->getType()->getTypeID()) {
						case llvm::Type::FloatTyID:
							p.getval(int_val, "latency::fadd::float");
							break;
						case llvm::Type::DoubleTyID:
							p.getval(int_val, "latency::fadd::double");
							break;
				// FIXME
						default:
							break;
					} // switch

					std::cerr << "Found fadd" << std::endl;
					llvm::errs() << "latency: " << int_val << "\n";
					inst = new instruction_t(int_val);
					break;

				case llvm::Instruction::Alloca:
					{
					std::cerr << "Found alloca" << std::endl;
					inst = new instruction_t(1);
					llvm::AllocaInst * ainst = llvm::cast<llvm::AllocaInst>(&*ita);
					llvm::Value * asize = ainst->getArraySize();
					llvm::errs() << "size: " << *asize << "\n";
					} // scope
					break;

				case llvm::Instruction::Load:
					std::cerr << "Found load" << std::endl;
					break;
			
				case llvm::Instruction::GetElementPtr:
					std::cerr << "Found getelementptr" << std::endl;
					break;
			
				// FIXME
				default:
					break;
			} // switch

			// add dependencies
			unsigned operands = ita->getNumOperands();
			for(unsigned i(0); i<operands; ++i) {
				auto op = processed.find(ita->getOperand(i));
				if(op != processed.end()) {
					inst->add_dependency(op->second);
				} // if
			} // for
			
			processed[value] = inst;
			active.push_back(inst);
			++ita;
		} // while
	} // for
} // parser_t::parser_t

} // namespace atlas

#endif // Parser_hh

/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#ifndef Parser_hh
#define Parser_hh

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <vector>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Instruction.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Support/InstIterator.h>

namespace atlas {

class parser_t
{
public:

	parser_t(const char * ir_file);
	~parser_t() {}

private:

	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

}; // class parser_t

class instruction_t
{
public:

	enum state_t {
		pending,
		executing,
		completed,
		stalled
	}; // enum state_t

	instruction_t(size_t latency)
		: state_(pending), latency_(latency), cycles_(0) {}

	void add_dependency(instruction_t * inst) {
		depends_.push_back(inst);
	} // add_dependency

	void operator ()(size_t cycle) {
	} // operator

private:

	state_t state_;
	size_t latency_;
	size_t cycles_;

	std::vector<instruction_t *> depends_;

}; // class instruction_t

parser_t::parser_t(const char * ir_file)
	: llvm_module_(nullptr)
{
	parameters_t & p = parameters_t::instance();
	std::map<llvm::Value *, instruction_t *> dependencies;

	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	// visit modules
	for(llvm::Module::iterator mita = llvm_module_->begin();
		mita != llvm_module_->end(); ++mita) {

		llvm::inst_iterator ita = inst_begin(mita);

		
		int32_t int_val;
		while(ita != inst_end(mita)) {
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
					} // switch


					std::cerr << "Found fadd" << std::endl;
					llvm::errs() << "latency: " << int_val << "\n";
					inst = new instruction_t(4);
					break;

				case llvm::Instruction::Alloca:
					std::cerr << "Found alloca" << std::endl;
					inst = new instruction_t(1);
					llvm::AllocaInst * ainst = llvm::cast<llvm::AllocaInst>(&*ita);
					llvm::Value * asize = ainst->getArraySize();
					llvm::errs() << "size: " << *asize << "\n";
					break;
			} // switch

			dependencies[value] = inst;
			++ita;
		} //

#if 0
		// visit instructions
		for (llvm::inst_iterator ita = inst_begin(mita), E = inst_end(mita);
			ita != E; ++ita) {
			llvm::Value * value = &*ita;

			std::cerr << "value address: " << value << std::endl;

			llvm::errs() << (*ita) << "\n";
			llvm::errs() << ita->getNumOperands() << "\n";
			
			switch(ita->getOpcode()) {
				case llvm::Instruction::FAdd:
					std::cerr << "Found Add" << std::endl;
					llvm::Value * opval = &*(ita->getOperand(0));
					std::cerr << "operand address: " << opval << std::endl;
					llvm::errs() << "Op 0: " << *(ita->getOperand(0)) << "\n";
					llvm::errs() << "Op 1: " << *(ita->getOperand(1)) << "\n";
					break;
			} // switch

#if 0
			for(size_t i(0); i<ita->getNumOperands(); ++i) {
				llvm::Value * op = ita->getOperand(i);
				llvm::errs() << "OP " << *op << "\n";
			} // for
#endif
		} // for
#endif
	} // for

} // parser_t::parser_t

} // namespace atlas

#endif // Parser_hh

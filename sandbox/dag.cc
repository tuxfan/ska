#include <iostream>

#include <llvm/Module.h>
#include <llvm/Constants.h>
#include <llvm/Function.h>
#include <llvm/Instruction.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/SCCIterator.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Support/InstIterator.h>

#include <Decode.hh>

#define USAGE(s)															\
	std::cerr << "usage: " << (s) << " architecture ir" << std::endl;		\
	std::exit(1);

int main(int argc, char ** argv) {
	std::string program = argv[0];
	size_t pos = program.find_last_of('/');
	if(pos != std::string::npos) {
		program = program.substr(pos+1);
	} // if

	if(argc < 3) {
		USAGE(program.c_str());
	} // if

	ska::parameters_t::instance().init(argv[1], false);

	std::string ir = argv[2];

	std::cerr << "Input file: " << ir.c_str() << std::endl;

	llvm::SMDiagnostic diagnostic;
	llvm::LLVMContext context;
	llvm::Module * module = ParseIRFile(ir.c_str(), diagnostic,
		context);

	if(module == nullptr) {
		std::cerr << "LLVM parse failed on " << ir.c_str() << std::endl <<
			diagnostic.getMessage() << std::endl;
	} // if

	llvm::Value * value = nullptr;
	ska::instruction_t * inst = nullptr;

	// iterate over functions
	for(auto fita = module->begin(); fita != module->end(); ++fita) {

		// iterate over basic blocks
		for(auto bita = fita->begin(); bita != fita->end(); ++bita) {
			std::map<llvm::Value *, ska::instruction_t *> processed;

			llvm::errs() << "Basic Block: " << bita->getName() << " has " <<
				bita->size() << " instructions\n";

			for(auto iita = bita->begin(); iita != bita->end(); ++iita) {
				value = &*iita;	
				inst = new ska::instruction_t(ska::decode(&*iita));

				for(unsigned i(0); i<iita->getNumOperands(); ++i) {
					auto op = processed.find(iita->getOperand(i));
					if(op != processed.end()) {
						inst->add_dependency(op->second);
					} // if
				} // for

				inst->update_strahler_number();
				std::cerr << "strahler: " << inst->strahler_number() << std::endl;
				processed[value] = inst;
			} // for
		} // for
	} // for

	//const std::vector<ska::instruction_t *> & deps = inst->dependencies();
	
	return 0;
} // main

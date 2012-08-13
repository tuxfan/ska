#include <iostream>

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

#define USAGE(s)															\
	std::cerr << "usage: " << (s) << " ir" << std::endl;		\
	std::exit(1);

int main(int argc, char ** argv) {
	std::string program = argv[0];
	size_t pos = program.find_last_of('/');
	if(pos != std::string::npos) {
		program = program.substr(pos+1);
	} // if

	if(argc < 2) {
		USAGE(program.c_str());
	} // if

	std::string ir = argv[1];

	std::cerr << "Input file: " << ir.c_str() << std::endl;

	llvm::SMDiagnostic diagnostic;
	llvm::LLVMContext context;
	llvm::Module * module = ParseIRFile(ir.c_str(), diagnostic,
		context);

	if(module == nullptr) {
		std::cerr << "LLVM parse failed on " << ir.c_str() << std::endl <<
			diagnostic.getMessage() << std::endl;
	} // if



	return 0;
} // main

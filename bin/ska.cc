/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#include <iostream>
#include <MachineState.hh>
#include <Parameters.hh>
#include <Parser.hh>

int main(int argc, char *argv[]) {

	atlas::parameters_t & gp = atlas::parameters_t::instance();

	if(argc != 3) {
		std::cerr << "Usage: " << argv[0] <<
			" <architecture file> <llvm ir file>" << std::endl;
		std::exit(1);
	} // if

	gp.init(argv[1], false);

	atlas::parser_t p(argv[2]);

	return 0;
} // main

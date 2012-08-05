/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#include <iostream>

#include <MachineState.hh>
#include <Parameters.hh>
#include <Simulator.hh>

int main(int argc, char ** argv) {

	if(argc != 3) {
		std::cerr << "Usage: " << argv[0] <<
			" <architecture file> <llvm ir file>" << std::endl;
		std::exit(1);
	} // if

	ska::parameters_t & gp = ska::parameters_t::instance();
	gp.init(argv[1], false);

	ska::simulator_t p(argv[2]);

	return 0;
} // main

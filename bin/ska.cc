/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved.
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Main ska executable.
 *----------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <unistd.h>

#include <MachineState.hh>
#include <Parameters.hh>
#include <Simulator.hh>

#define USAGE(s)															\
	std::cerr << "usage: " << (s) << " [-o output file]" <<	\
	" architecture" << " ir" << std::endl;							\
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

	/*-------------------------------------------------------------------------*
	 * Parse input arguments.
	 *-------------------------------------------------------------------------*/

	int ch;
	extern char * optarg;
	extern int optind;

	std::streambuf * stream_buf = std::cerr.rdbuf();
	std::ofstream file;

	while((ch = getopt(argc, argv, "o:")) != -1) {
		switch(ch) {
			case 'o':
				file.open(optarg);

				if(!file.good()) {
					std::cerr << "Failed opening " << optarg << std::endl;
					std::exit(1);
				} // if

				stream_buf = file.rdbuf();
				break;
			default:
				USAGE(program.c_str());
		} // switch
	} // while

	std::ostream stream(stream_buf);

	argc -= optind;
	argv += optind;

	/*-------------------------------------------------------------------------*
	 * Read architecture specification.
	 *-------------------------------------------------------------------------*/

	ska::parameters_t & gp = ska::parameters_t::instance();
	gp.init(argv[0], false);

	/*-------------------------------------------------------------------------*
	 * Call simulator.
	 *-------------------------------------------------------------------------*/

	ska::simulator_t p(argv[1], stream);

	/*-------------------------------------------------------------------------*
	 * Close output file.
	 *-------------------------------------------------------------------------*/

	if(file.is_open()) {
		file.close();
	} // if

	return 0;
} // main

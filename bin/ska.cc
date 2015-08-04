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

#define USAGE(s)																				\
	std::cerr << "Usage: " << (s) << " -v [-l log file]" <<						\
	" [-o output file]" <<	" architecture ir" <<									\
	std::endl << std::endl << "Options:" << std::endl <<							\
	"  -d      Turn on debug output (default: stderr)" << std::endl <<		\
	"          (turns off normal logging output)" << std::endl <<				\
	"  -v      Turn on verbose output (default: stderr)" << std::endl <<		\
	"  -l file Specify a file for log output (implies -v)" << std::endl <<	\
	"  -o file Specify a file for pipeline output" << std::endl;				\
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

	while((ch = getopt(argc, argv, "dvo:l:")) != -1) {
		switch(ch) {
			case 'o':
				ska::file_io_t::instance().set_out_stream(optarg);
				break;
			case 'l':
				ska::file_io_t::instance().set_log_stream(optarg);
				break;
			case 'v':
				ska::file_io_t::instance().set_verbose();
				break;
			case 'd':
				ska::file_io_t::instance().set_debug();
				break;
			default:
				USAGE(program.c_str());
		} // switch
	} // while

	argc -= optind;
	argv += optind;

	/*-------------------------------------------------------------------------*
	 * Read architecture specification.
	 *-------------------------------------------------------------------------*/

	std::string archfile = ska::try_file(argv[0], "SKA_ARCH_PATH");
	ska::parameters_t::instance().init(archfile.c_str(), false);

	/*-------------------------------------------------------------------------*
	 * Call simulator.
	 *-------------------------------------------------------------------------*/

	ska::simulator_t p(argv[1]);

	return 0;
} // main

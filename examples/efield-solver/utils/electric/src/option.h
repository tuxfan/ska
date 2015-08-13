#ifndef OPTION_H
#define OPTION_H

#include "problem.h"
#include "map.h"


typedef struct {
	problem_id problem;
	map_id map;
	int periodic;
	int eig;
	int nx, ny, nz;
} option;


option parse_options(int argc, char *argv[]);


void print_intro(option *);


#endif

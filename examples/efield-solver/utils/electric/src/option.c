#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>

#include "option.h"


void print_problems()
{
	int rank, i;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Available Problems\n");
	printf("==================\n");
	for (i = 0; i < NUM_PROBLEMS; i++)
		printf("%s (%s)\n", problem_name[i], problem_key[i]);
	printf("\n");
}


void print_maps()
{
	int rank, i;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("Available Maps\n");
	printf("==============\n");
	for (i = 0; i < NUM_MAPS; i++)
		printf("%s (%s)\n", map_name[i], map_key[i]);
	printf("\n");
}


option parse_options(int argc, char *argv[])
{
	option op;
	int c, i;

	op.nx = op.ny = 301;
	op.nz = 0;
	op.eig = 0;
	op.problem = SIN;
	op.map = MAP_IDENTITY;
	op.periodic = -1;

	opterr = 0;

	while ((c = getopt(argc, argv, "x:y:z:m:p:l:w:s:d:e")) != -1) {
		switch (c) {
		case 'e':
			op.eig = 1;
			break;
		case 'x':
			op.nx = atoi(optarg);
			break;
		case 'y':
			op.ny = atoi(optarg);
			break;
		case 'z':
			op.nz = atoi(optarg);
			break;
		case 's':
			op.nx = op.ny = op.nz = atoi(optarg);
			break;
		case 'd':
			op.nx = op.ny = atoi(optarg);
			break;
		case 'w':
			if (optarg[0] == 'x')
				op.periodic = 0;
			else if (optarg[0] == 'y')
				op.periodic = 1;
			else if (optarg[0] == 'z')
				op.periodic = 2;
			break;
		case 'm':
			for (i = 0; i < NUM_MAPS; i++) {
				if (strcmp(optarg, map_key[i]) == 0)
					op.map = i;
			}
			break;
		case 'p':
			for (i = 0; i < NUM_PROBLEMS; i++) {
				if (strcmp(optarg, problem_key[i]) == 0)
					op.problem = i;
			}
			break;
		case 'l':
			if (optarg[0] == 'p') {
				print_problems();
				MPI_Finalize();
				exit(0);
			} else if (optarg[0] == 'm') {
				print_maps();
				MPI_Finalize();
				exit(0);
			}
			break;
		case '?':
			if (optopt == 'x'
			    || optopt == 'y'
			    || optopt == 'z'
			    || optopt == 'm'
			    || optopt == 'w'
			    || optopt == 's'
			    || optopt == 'd'
			    || optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return op;
		default:
			abort();
		}
	}

	return op;
}


void print_intro(option *op)
{
	const char dim_name[] = {'x', 'y', 'z'};
	printf("Running Efield Test\n");
	printf("===================\n");
	printf("nx:       %d\n", op->nx);
	printf("ny:       %d\n", op->ny);
	printf("nz:       %d\n", op->nz);
	printf("problem:  %s\n", problem_name[op->problem]);
	printf("map:      %s\n", map_name[op->map]);
	if (op->periodic > -1)
		printf("periodic: %c\n", dim_name[op->periodic]);
	printf("\n");
}

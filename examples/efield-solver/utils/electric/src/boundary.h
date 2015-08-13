#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "grid.h"
#include "problem.h"

typedef enum {
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3,
	BACK = 4,
	FRONT = 5
} bdir;

typedef enum {
	DIRICHLET=300, NEUMANN=302
} btype;

typedef struct {
	int is[3];
	int ie[3];
	btype type;
	double *dirichlet;
	int norm_dir;
} boundary;

typedef struct {
	boundary *v;
	int len;
} blist;

blist *boundary_create(grid*, problem*);

void boundary_destroy(blist*);

#endif

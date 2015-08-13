#ifndef STATE_H
#define STATE_H

#include "grid.h"
#include "problem.h"

typedef struct {
	double *phi;
	double *rhs;
	double *eps;
	double *debye;
	double *sol;
} state;

state *state_create(grid*, problem*);

void state_destroy(state*);

#endif

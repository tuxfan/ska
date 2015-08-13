#ifndef SOLVER_H
#define SOLVER_H

#include "ef_interface.h"

#include "boundary.h"
#include "state.h"
#include "grid.h"
#include "problem.h"


typedef struct {
	blist *bnd;
	state *state;
	void  *ptr;
} solver;

solver *solver_create(grid*, problem*);

PetscErrorCode solver_init(solver*, grid*);

PetscErrorCode solver_run(solver*);

PetscErrorCode solver_destroy(solver*);

#endif

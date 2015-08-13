#include <stdlib.h>
#include "state.h"

state *state_create(grid *grd, problem *pb)
{
	int i;

	state *st = (state*) malloc(sizeof(state));
	st->rhs = (double*) malloc(grd->num_pts*sizeof(double));
	st->phi = (double*) malloc(grd->num_pts*sizeof(double));
	st->eps = (double*) malloc(grd->num_pts*sizeof(double));
	st->debye = (double*) malloc(grd->num_pts*sizeof(double));
	st->sol = (double*) malloc(grd->num_pts*sizeof(double));

	if (pb->id == JUMP) {
		for (i = 0; i < grd->num_pts; i++) {
			if (grd->x[i] <= 0)
				st->eps[i] = 4;
			else
				st->eps[i] = 2;
		}
	} else {
		for (i = 0; i < grd->num_pts; i++) {
			st->eps[i] = 1.0;
		}
	}

	grid_eval(grd, pb->rhs, st->rhs);
	grid_eval(grd, pb->sol, st->sol);

	return st;
}


void state_destroy(state* st)
{
	free(st->rhs);
	free(st->phi);
	free(st->eps);
	free(st->debye);
}

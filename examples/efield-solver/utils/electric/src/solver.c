#include <stdlib.h>

#include "solver.h"
#include "problem.h"
#include "grid.h"


solver *solver_create(grid *grd, problem *pb)
{
	solver *slv = (solver*) malloc(sizeof(solver));
	slv->bnd = boundary_create(grd, pb);
	slv->state = state_create(grd, pb);

	return slv;
}


PetscErrorCode solver_init(solver *slv, grid *grd)
{
	PetscErrorCode ierr;
	int i;
	int stride[3], offset[3];

	blist *bnd = slv->bnd;

	for (i = 0; i < 3; i++) offset[i] = 0;
	stride[0] = grd->nx; stride[1] = grd->ny; stride[2] = grd->nz;

	ierr = PetscInitialize(NULL, NULL, "PETScOptions.txt", NULL);CHKERRQ(ierr);
	ef_init_(&slv->ptr, &grd->comm, grd->num_global, grd->num_procs,
	         grd->num_local,
	         offset, stride,
	         grd->cart_coord, grd->periodic, &grd->nd, &grd->id,
	         &ierr);CHKERRQ(ierr);
	ef_set_grid_(&slv->ptr, grd->is, grd->ie, &grd->num_pts, &grd->xyz,
	             &ierr);CHKERRQ(ierr);
	ef_set_state_(&slv->ptr, &slv->state->rhs, &slv->state->phi, &slv->state->eps,
	              &slv->state->debye, &ierr);CHKERRQ(ierr);
	ef_set_sol_(&slv->ptr, slv->state->sol, &ierr);CHKERRQ(ierr);

	for(i = 0; i < bnd->len; i++) {
		ef_set_patch_(&slv->ptr, bnd->v[i].is, bnd->v[i].ie, (int*) &bnd->v[i].type,
		              &bnd->v[i].norm_dir, &bnd->v[i].dirichlet);
	}

	return 0;
}


PetscErrorCode solver_run(solver *slv)
{
	PetscErrorCode ierr;

	ef_solve_(&slv->ptr, &ierr);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode solver_destroy(solver *slv)
{
	PetscErrorCode ierr;

	ef_cleanup_(&slv->ptr, &ierr);CHKERRQ(ierr);
	boundary_destroy(slv->bnd);
	free(slv->bnd);
	state_destroy(slv->state);
	free(slv->state);

	ierr = PetscFinalize();CHKERRQ(ierr);

	return 0;
}

#include "ef_boundary.h"


PetscErrorCode ef_boundary_create(ef_boundary **ef_bnd, ef_level levels[],
                                  int nlevels,
                                  ef_dmap *dmap, ef_state *state, ef_fd *fd)
{
	ef_boundary *bnd = (ef_boundary*) malloc(sizeof(ef_boundary));

	bnd->bc = NULL;
	bnd->levels = levels;
	bnd->num_levels = nlevels;
	bnd->slv_dmap = dmap;
	bnd->state = state;
	bnd->fd = fd;
	bnd->num_bc = 0;
	bnd->axisymmetric = 0;

	*ef_bnd = bnd;
	return 0;
}


PetscErrorCode ef_boundary_apply(ef_boundary *bnd, Mat A, DM da)
{
	PetscErrorCode ierr;
	int i;
	PetscInt level;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	for (i = 0; i < bnd->num_bc; i++) {
		if (bnd->bc[i]->patch->bc_type == EF_NEUMANN) {
			ierr = ef_bc_apply(bnd->bc[i], A, da);CHKERRQ(ierr);
		}
	}
	for (i = 0; i < bnd->num_bc; i++) {
		if (bnd->bc[i]->patch->bc_type == EF_DIRICHLET_G ||
		    bnd->bc[i]->patch->bc_type == EF_DIRICHLET_P) {
			ierr = ef_bc_apply(bnd->bc[i], A, da);CHKERRQ(ierr);
		}
	}

	if (level) { // prepare rhs transform. for coarse levels
		ierr = ef_boundary_apply_rhs(bnd, da);CHKERRQ(ierr);
	}

	// temporary symmetric Dirchlet hack
	ierr = MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatMult(A, bnd->levels[level].g, bnd->levels[level].ag);CHKERRQ(ierr);

	for (i = 0; i < bnd->num_bc; i++) {
		ierr = ef_bc_symmetric(bnd->bc[i], A, da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_boundary_apply_rhs(ef_boundary *bnd, DM da)
{
	PetscErrorCode ierr;
	int i;

	for (i = 0; i < bnd->num_bc; i++) {
		ierr = ef_bc_apply_rhs(bnd->bc[i], da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_boundary_add(ef_boundary *bnd, ef_bctype btype, int norm_dir,
                               int is[], int ie[], double *dirichlet)
{
	PetscErrorCode ierr;
	int ind = bnd->num_bc;

	if ((bnd->num_bc % 10) == 0) {
		bnd->bc = realloc(bnd->bc, (bnd->num_bc + 10) * sizeof(ef_bc*));
	}

	ierr = ef_bc_create(&bnd->bc[ind], btype, norm_dir, is, ie, dirichlet,
	                    bnd->levels, bnd->slv_dmap, bnd->state, bnd->fd,
	                    bnd->num_levels);CHKERRQ(ierr);
	bnd->bc[ind]->axisymmetric = bnd->axisymmetric;
	bnd->num_bc++;

	return 0;
}


PetscErrorCode ef_boundary_coarsen(ef_boundary *bnd, ef_grid *grid, DM dmc)
{
	PetscErrorCode ierr;
	int i;

	for (i = 0; i < bnd->num_bc; i++) {
		ierr = bnd->bc[i]->coarsen(bnd->bc[i], grid, dmc);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_boundary_destroy(ef_boundary *bnd)
{
	PetscErrorCode ierr;
	int i;

	for (i = 0; i < bnd->num_bc; i++) {
		ierr = ef_bc_destroy(bnd->bc[i]);CHKERRQ(ierr);
	}

	return 0;
}

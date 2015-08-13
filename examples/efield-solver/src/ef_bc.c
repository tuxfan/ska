#include "ef_bc.h"
#include "ef_neumann.h"
#include "ef_dirichlet.h"


static PetscErrorCode coarsen(ef_bc *bc, ef_grid *grid, DM da)
{
	PetscErrorCode ierr;
	PetscInt mxs, mys, mxm, mym, xm, ym;
	PetscInt level;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);


	ierr = DMDAGetCorners(da, &mxs, &mys, 0, &mxm, &mym, 0);CHKERRQ(ierr);
	ym = patch->gie[1] - patch->gis[1] + 1;
	xm = patch->gie[0] - patch->gis[0] + 1;

	if ((patch->gis[1] == grid->is[1] && patch->gie[1] == grid->ie[1])
	    && (patch->gis[0] == grid->is[0] || patch->gie[0] == grid->ie[0])
	    && (xm == 1)) {
		patch->corners[level].is[1] = mys; patch->corners[level].ie[1] = mys + mym - 1;
		if (patch->gis[0] == grid->is[0]) {
			patch->corners[level].is[0] = mxs;
			patch->corners[level].ie[0] = patch->corners[level].is[0];
		} else {
			patch->corners[level].is[0] = mxs + mxm - 1;
			patch->corners[level].ie[0] = patch->corners[level].is[0];
		}
	} else if ((patch->gis[0] == grid->is[0] && patch->gie[0] == grid->ie[0])
	           && (patch->gis[1] == grid->is[1] || patch->gie[1] == grid->ie[1])
	           && (ym == 1)) {
		patch->corners[level].is[0] = mxs; patch->corners[level].ie[0] = mxs + mxm - 1;
		if (patch->gis[1] == grid->is[1]) {
			patch->corners[level].is[1] = mys;
			patch->corners[level].ie[1] = patch->corners[level].is[1];
		} else {
			patch->corners[level].is[1] = mys + mym - 1;
			patch->corners[level].ie[1] = patch->corners[level].is[1];
		}
	} else {
		SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_SUP, "Interior boundary on level > 0 not yet supported!");
	}


	return 0;
}


static PetscErrorCode coarsen_3d(ef_bc *bc, ef_grid *grid, DM da)
{
	PetscErrorCode ierr;
	PetscInt mxs, mys, mzs, mxm, mym, mzm, xm, ym, zm;
	ef_patch *patch = bc->patch;
	PetscInt level;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);


	ierr = DMDAGetCorners(da, &mxs, &mys, &mzs, &mxm, &mym, &mzm);CHKERRQ(ierr);
	ym = patch->gie[1] - patch->gis[1] + 1;
	xm = patch->gie[0] - patch->gis[0] + 1;
	zm = patch->gie[2] - patch->gis[2] + 1;

	if ((patch->gis[1] == grid->is[1] && patch->gie[1] == grid->ie[1])
	    && (patch->gis[2] == grid->is[2] && patch->gie[2] == grid->ie[2])
	    && (patch->gis[0] == grid->is[0] || patch->gie[0] == grid->ie[0])
	    && (xm == 1)) {
		patch->corners[level].is[1] = mys; patch->corners[level].ie[1] = mys + mym - 1;
		patch->corners[level].is[2] = mzs; patch->corners[level].ie[2] = mzs + mzm - 1;
		if (patch->gis[0] == grid->is[0]) {
			patch->corners[level].is[0] = mxs;
			patch->corners[level].ie[0] = patch->corners[level].is[0];
		} else {
			patch->corners[level].is[0] = mxs + mxm - 1;
			patch->corners[level].ie[0] = patch->corners[level].is[0];
		}
	} else if ((patch->gis[0] == grid->is[0] && patch->gie[0] == grid->ie[0])
	           && (patch->gis[2] == grid->is[2] && patch->gie[2] == grid->ie[2])
	           && (patch->gis[1] == grid->is[1] || patch->gie[1] == grid->ie[1])
	           && (ym == 1)) {
		patch->corners[level].is[0] = mxs; patch->corners[level].ie[0] = mxs + mxm - 1;
		patch->corners[level].is[2] = mzs; patch->corners[level].ie[2] = mzs + mzm - 1;
		if (patch->gis[1] == grid->is[1]) {
			patch->corners[level].is[1] = mys;
			patch->corners[level].ie[1] = patch->corners[level].is[1];
		} else {
			patch->corners[level].is[1] = mys + mym - 1;
			patch->corners[level].ie[1] = patch->corners[level].is[1];
		}
	} else if ((patch->gis[0] == grid->is[0] && patch->gie[0] == grid->ie[0])
	           && (patch->gis[1] == grid->is[1] && patch->gie[1] == grid->ie[1])
	           && (patch->gis[2] == grid->is[2] || patch->gie[2] == grid->ie[2])
	           && (zm == 1)) {
		patch->corners[level].is[0] = mxs; patch->corners[level].ie[0] = mxs + mxm - 1;
		patch->corners[level].is[1] = mys; patch->corners[level].ie[1] = mys + mym - 1;
		if (patch->gis[2] == grid->is[2]) {
			patch->corners[level].is[2] = mzs;
			patch->corners[level].ie[2] = patch->corners[level].is[2];
		} else {
			patch->corners[level].is[2] = mzs + mzm - 1;
			patch->corners[level].ie[2] = patch->corners[level].is[2];
		}
	} else {
		SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_SUP, "Interior boundary on level > 0 not yet supported!");
	}

	return 0;
}


PetscErrorCode ef_bc_create(ef_bc **efbc, ef_bctype btype, int norm_dir, int is[], int ie[],
                            double *dirichlet, ef_level levels[], ef_dmap *dmap,
                            ef_state *state, ef_fd *fd, int nlevels)
{
	PetscErrorCode ierr;
	int i;

	ef_bc *bc = (ef_bc*) malloc(sizeof(ef_bc));
	bc->patch = (ef_patch*) malloc(sizeof(ef_patch));
	bc->patch->corners = (ef_corner*) malloc(nlevels*sizeof(ef_corner));
	bc->patch->gis = is;
	bc->patch->gie = ie;
	for (i = 0; i < 3; i++) {
		bc->patch->corners[0].is[i] = is[i] - 1;
		bc->patch->corners[0].ie[i] = ie[i] - 1;
	}
	bc->patch->bc_type = btype;
	bc->patch->norm_dir = norm_dir;
	bc->patch->dirichlet = dirichlet;
	bc->num_levels = nlevels;
	bc->levels = levels;
	bc->slv_dmap = dmap;
	bc->state = state;
	bc->fd = fd;
	bc->axisymmetric = 0;
	bc->destroy = NULL;
	bc->symmetric = NULL;
	bc->apply = NULL;
	bc->apply_rhs = NULL;

	if (dmap->nd == 2)
		bc->coarsen = &coarsen;
	else
		bc->coarsen = &coarsen_3d;

	if (btype == EF_DIRICHLET_G || btype == EF_DIRICHLET_P) {
		ierr = ef_dirichlet_create(bc);CHKERRQ(ierr);
	} else if (btype == EF_NEUMANN) {
		ierr = ef_neumann_create(bc);CHKERRQ(ierr);
	}

	*efbc = bc;

	return 0;
}


PetscErrorCode ef_bc_symmetric(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;

	if (bc->symmetric) {
		ierr = bc->symmetric(bc, A, da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_bc_apply(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;

	if (bc->apply) {
		ierr = bc->apply(bc, A, da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_bc_apply_rhs(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;

	if (bc->apply_rhs) {
		ierr = bc->apply_rhs(bc, da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_bc_destroy(ef_bc *bc)
{
	PetscErrorCode ierr;

	if (bc->destroy) {
		ierr = bc->destroy(bc);CHKERRQ(ierr);
	}
	free(bc->patch->corners);
	free(bc->patch);

	return 0;
}

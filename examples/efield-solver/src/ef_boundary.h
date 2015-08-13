#ifndef EF_BOUNDARY_H
#define EF_BOUNDARY_H

#include "ef_bc.h"


typedef struct ef_boundary {
	ef_bc **bc;
	int num_bc;
	ef_level *levels;
	int num_levels;
	ef_dmap *slv_dmap;
	ef_state *state;
	int axisymmetric;
	ef_fd *fd;
} ef_boundary;


PetscErrorCode ef_boundary_create(ef_boundary **ef_bnd, ef_level levels[],
                                  int nlevels,
                                  ef_dmap *dmap, ef_state *state, ef_fd *fd);


PetscErrorCode ef_boundary_apply(ef_boundary *bnd, Mat A, DM da);


PetscErrorCode ef_boundary_add(ef_boundary *bnd, ef_bctype btype, int norm_dir,
                               int is[], int ie[], double *dirichlet);


PetscErrorCode ef_boundary_apply_rhs(ef_boundary *bnd, DM da);


PetscErrorCode ef_boundary_coarsen(ef_boundary *bnd, ef_grid *grid, DM dmc);


PetscErrorCode ef_boundary_destroy(ef_boundary *bnd);


#endif

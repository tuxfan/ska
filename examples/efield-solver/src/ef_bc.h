#ifndef EF_BC_H
#define EF_BC_H

#include <petscdmda.h>
#include <petscmat.h>
#include <petscvec.h>

#include "ef_level.h"
#include "ef_dmap.h"
#include "ef_state.h"
#include "ef_grid.h"

typedef enum {
	EF_DIRICHLET_G = 300,
	EF_DIRICHLET_P = 301,
	EF_NEUMANN = 302
} ef_bctype;


typedef struct {
	int is[3], ie[3];
} ef_corner;


typedef struct {
	int *gis, *gie;
	ef_corner *corners;
	ef_bctype bc_type;
	double *dirichlet;
	int norm_dir;
} ef_patch;


typedef struct ef_bc_ {
	PetscErrorCode (*apply)(struct ef_bc_ *bc, Mat A, DM da);
	PetscErrorCode (*apply_rhs)(struct ef_bc_ *bc, DM da);
	PetscErrorCode (*coarsen)(struct ef_bc_ *bc, ef_grid *grid, DM da);
	PetscErrorCode (*symmetric)(struct ef_bc_ *bc, Mat A, DM da);
	PetscErrorCode (*destroy)(struct ef_bc_ *bc);
	ef_level *levels;
	int num_levels;
	int axisymmetric;
	ef_dmap *slv_dmap;
	ef_patch *patch;
	ef_state *state;
	ef_fd *fd;
	void *sub;
} ef_bc;


PetscErrorCode ef_bc_create(ef_bc **efbc, ef_bctype btype, int norm_dir, int is[], int ie[],
                            double *dirichlet, ef_level levels[], ef_dmap *dmap,
                            ef_state *state, ef_fd *fd, int nlevels);


PetscErrorCode ef_bc_apply(ef_bc *bc, Mat A, DM da);


PetscErrorCode ef_bc_apply_rhs(ef_bc *bc, DM da);


PetscErrorCode ef_bc_symmetric(ef_bc *bc, Mat A, DM da);


PetscErrorCode ef_bc_destroy(ef_bc *bc);


#endif

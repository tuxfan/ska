#ifndef EF_SOLVER_H
#define EF_SOLVER_H

#include <petscdmda.h>
#include <petscksp.h>

#include "ef_dmap.h"
#include "ef_grid.h"
#include "ef_metric.h"
#include "ef_operator.h"
#include "ef_boundary.h"
#include "ef_level.h"
#include "ef_state.h"
#include "ef_fd.h"
#include "ef_callback.h"


typedef enum {
	EFS_LOG_STATUS   = 1,
	EFS_LOG_PROBLEM  = (1<<1),
	EFS_LOG_RESIDUAL = (1<<2),
	EFS_LOG_VTK      = (1<<3),
	EFS_LOG_EIGS     = (1<<4),
	EFS_LOG_ALL      = (1<<5)-1
} efs_log_level;


typedef struct {
	int matfree;
	int axisymmetric;
	PetscBool galerkin;
	efs_log_level log_level;
	PetscInt levels;
} efs_option;


typedef struct {
	Mat A;
	DM  dm;
	KSP ksp;
	PC pc;
	Vec x;
	Vec rhs;
	Vec sol;
	MPI_Comm comm;
	ef_state   state;
	ef_grid    grid;
	efs_option options;
	ef_level *levels;
	ef_dmap  *dmap;
	ef_operator *op;
	ef_boundary *boundary;
	ef_fd *fd;
	ef_callback *callback;
	int num_patches;
	int ts;
} efs;


PetscErrorCode efs_create(efs**, MPI_Comm);


PetscErrorCode efs_setup(efs*, int offset[], int stride[]);


PetscErrorCode efs_solve(efs*);


PetscErrorCode efs_set_state(efs *slv, double rhs[], double phi[], double eps[],
                             double debye[]);


PetscErrorCode efs_set_sol(efs *slv, double sol[]);


PetscErrorCode efs_set_grid(efs *slv, int is[], int ie[], int num_cells, double xyz[]);


int efs_log(efs*, efs_log_level);


void efs_set_log(efs*, efs_log_level);


PetscErrorCode efs_destroy(efs*);

#endif

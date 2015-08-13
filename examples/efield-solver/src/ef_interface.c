#include <stdio.h>
#include <stdlib.h>

#include "ef_solver.h"
#include "ef_io.h"


void ef_set_patch_(void **solver, int is[], int ie[], int *bc_type,
                   int *norm_dir, double **dirichlet)

{
	efs *slv = (efs*) *solver;

	ef_boundary_add(slv->boundary, (ef_bctype) *bc_type, *norm_dir,
	                is, ie, *dirichlet);
}


void ef_init_(void **solverCtx, MPI_Fint *fcomm,
              int nGlobal[], int nProcs[], int nLocal[],
              int offset[], int stride[],
              int cartCoord[], int periodic[], int *nd,
              int *ng, PetscErrorCode *ierr)
{
	efs *slv;

	MPI_Comm *comm = malloc(sizeof(MPI_Comm));
	#ifdef C_CLIENT
	*comm = *fcomm;
	#else
	*comm = MPI_Comm_f2c(*fcomm);
	#endif
	*ierr = efs_create(&slv, *comm);
	slv->options.axisymmetric = 0;
	efs_set_log(slv, EFS_LOG_VTK | EFS_LOG_STATUS | EFS_LOG_RESIDUAL);
	*ierr = ef_grid_setup(&slv->grid, &slv->dm, &slv->comm, nGlobal, nProcs,
	                      nLocal, cartCoord, periodic, *nd);
	slv->grid.id = *ng;
	*ierr = efs_setup(slv, offset, stride);

	*solverCtx = slv;
}


void ef_set_state_(void **solver, double **rhs, double **phi, double **eps,
                   double **debye, PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = efs_set_state(slv, *rhs, *phi, *eps, *debye);
}


void ef_set_sol_(void **solver, double sol[], PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = efs_set_sol(slv, sol);
}


void ef_set_grid_(void **solver, int is[], int ie[], int *nCells, double **xyz,
                  PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = efs_set_grid(slv, is, ie, *nCells, *xyz);
}


void ef_solve_(void **solver, PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = efs_solve(slv);
}


void ef_cleanup_(void **solver, PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = efs_destroy(slv);
	free(slv);
}


void ef_plot_sol_(void **solver, int *idx, PetscErrorCode *ierr)
{
	efs *slv = (efs*) *solver;

	*ierr = ef_io_vtkwrite(slv->dm, slv->sol, "solution", slv->grid.id, *idx);
}

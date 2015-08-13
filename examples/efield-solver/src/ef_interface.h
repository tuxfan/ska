#ifndef EF_INTERFACE_H
#define EF_INTERFACE_H

#include "ef_solver.h"
#include "ef_bc.h"


void ef_init_(void **solverCtx, MPI_Comm *comm,
              int nGlobal[], int nProcs[], int nLocal[],
              int offset[], int stride[],
              int cartCoord[], int periodic[], int *nd,
              int *ng, PetscErrorCode *ierr);


void ef_set_state_(void **solver_ctx, double **rhs, double **phi, double **eps,
                   double **debye, PetscErrorCode *ierr);

void ef_set_sol_(void **solver_ctx, double sol[], PetscErrorCode *ierr);

void ef_set_grid_(void **solver_ctx, int is[], int ie[], int *nCells,
                  double **xyz, PetscErrorCode *ierr);

void ef_solve_(void **solver_ctx, PetscErrorCode *ierr);

void ef_set_patch_(void **solver_ctx, int is[], int ie[], int *bc_type,
                   int *norm_dir, double **dirichlet);

void ef_cleanup_(void **solver_ctx, PetscErrorCode *ierr);
#endif

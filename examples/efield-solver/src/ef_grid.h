#ifndef EF_GRID_H
#define EF_GRID_H

#include <petscdmda.h>
#include "mpi.h"

typedef struct {
	int nd;
	int id;
	int is[3];
	int ie[3];
	int num_cells;
	double *xyz;
} ef_grid;


PetscErrorCode ef_grid_setup(ef_grid *grid, DM *dm, MPI_Comm *comm, int nGlobal[], int nProcs[], int nLocal[],
                             int cartCoord[], int periodic[], int nd);


#endif

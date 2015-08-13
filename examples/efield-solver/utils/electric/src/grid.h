
#ifndef GRID_H
#define GRID_H

#include "mpi.h"
#include "base.h"
#include "map.h"

typedef struct{
	int nd;
	double *x, *y, *z, *xyz;
	double hx, hy, hz;
	int    nx, ny, nz;
	int    num_pts;
	int    id;
	int    is[3];
	int    ie[3];
	int    num_global[3];
	int    num_local[3];
	int    cart_coord[3];
	int    num_procs[3];
	int    periodic[3];
	MPI_Comm comm;
} grid;

grid *grid_create(double startx, double endx, int nx,
                  double starty, double endy, int ny,
                  double startz, double endz, int nz);


void grid_apply_map(grid*, map*);


void grid_eval(grid*, func, double*);


void grid_print(grid*, double*);


void grid_destroy(grid*);

#endif

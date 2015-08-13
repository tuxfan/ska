#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#include "block.h"
#include "grid.h"


static int icmp(const void *a, const void *b)
{
	int *x = (int*) a;
	int *y = (int*) b;

	return *y - *x;
}


void grid_decomp(grid *grd, int np)
{
	// decomp from plascomcm
	int i, j, p, ind;
	int faclen;
	int maxlen;
	int *fac;
	double curr;
	double n[3];
	int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

	n[0] = grd->num_global[0];
	n[1] = grd->num_global[1];
	n[2] = grd->num_global[2];

	maxlen = log2(np) + 1;
	fac = (int*) malloc(maxlen*sizeof(int));
	p = 0;
	faclen = 0;
	for (i = 0; i < maxlen; i++) {
		if (np % primes[p] == 0) {
			fac[faclen] = primes[p];
			np = np / primes[p];
			faclen++;
		} else {
			p++;
			if (p > 11) break;
		}
	}

	if (np > 1) {
		fac[faclen] = np;
		faclen++;
	}

	qsort(fac, faclen, sizeof(int), icmp);

	for (i = 0; i < grd->nd; i++) grd->num_procs[i] = 1;

	for (i = 0; i < faclen; i++) {
		curr = 0;
		ind = 0;
		for (j = 0; j < grd->nd; j++) {
			if (n[j] > curr) {
				curr = n[j];
				ind = j;
			}
		}
		grd->num_procs[ind] = grd->num_procs[ind] * fac[i];
		n[ind] = ((double) n[ind]) / ((double) fac[i]);
	}

	free(fac);
}

grid *grid_create(double startx, double endx, int nx,
                  double starty, double endy, int ny,
                  double startz, double endz, int nz)
{
	MPI_Comm cart_comm;
	int periodics[3];
	int i, j, k;
	int ind;
	int np, rank;

	grid *grd = (grid*) malloc(sizeof(grid));

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	grd->id = 0;

	if (nz > 0)
		grd->nd = 3;
	else
		grd->nd = 2;

	grd->num_global[0] = nx;
	grd->num_global[1] = ny;
	grd->num_global[2] = nz;

	grid_decomp(grd, np);

	periodics[0] = periodics[1] = periodics[2] = 0;
	MPI_Cart_create(MPI_COMM_WORLD, grd->nd, grd->num_procs, periodics, 1, &cart_comm);
	MPI_Comm_rank(cart_comm, &rank);
	MPI_Cart_coords(cart_comm, rank, grd->nd, grd->cart_coord);

	grd->nx = BLOCK_SIZE(grd->cart_coord[0], grd->num_procs[0], nx);
	grd->ny = BLOCK_SIZE(grd->cart_coord[1], grd->num_procs[1], ny);

	grd->num_local[0] = grd->nx;
	grd->num_local[1] = grd->ny;

	grd->num_pts = grd->nx * grd->ny;
	grd->is[0] = BLOCK_LOW(grd->cart_coord[0], grd->num_procs[0], nx) + 1;
	grd->ie[0] = BLOCK_HIGH(grd->cart_coord[0], grd->num_procs[0], nx) + 1;
	grd->is[1] = BLOCK_LOW(grd->cart_coord[1], grd->num_procs[1], ny) + 1;
	grd->ie[1] = BLOCK_HIGH(grd->cart_coord[1], grd->num_procs[1], ny) + 1;

	if (grd->nd == 3) {
		grd->nz = BLOCK_SIZE(grd->cart_coord[2], grd->num_procs[2], nz);
		grd->num_local[2] = grd->nz;
		grd->num_pts = grd->num_pts * grd->nz;
		grd->is[2] = BLOCK_LOW(grd->cart_coord[2], grd->num_procs[2], nz) + 1;
		grd->ie[2] = BLOCK_HIGH(grd->cart_coord[2], grd->num_procs[2], nz) + 1;
	}

	grd->comm = MPI_COMM_WORLD;

	grd->xyz = (double*) malloc(grd->num_pts*3*sizeof(double));
	grd->x = &grd->xyz[0];
	grd->y = &grd->xyz[grd->num_pts];
	grd->z = &grd->xyz[grd->num_pts*2];

	grd->hx = (endx - startx) / (grd->num_global[0] - 1);
	grd->hy = (endy - starty) / (grd->num_global[1] - 1);
	grd->hz = (endz - startz) / (grd->num_global[2] - 1);

	startx = startx + (grd->is[0]-1)*grd->hx;
	starty = starty + (grd->is[1]-1)*grd->hy;
	startz = startz + (grd->is[2]-1)*grd->hz;

	if (grd->nd == 3) {
		for (k = 0; k < grd->nz; k++) {
			for (j = 0; j < grd->ny; j++) {
				for (i = 0; i < grd->nx; i++) {
					ind = k*grd->nx*grd->ny + j*grd->nx + i;
					grd->x[ind] = startx + i*grd->hx;
					grd->y[ind] = starty + j*grd->hy;
					grd->z[ind] = startz + k*grd->hz;
				}
			}
		}
	} else {
		for (j = 0; j < grd->ny; j++) {
			for (i = 0; i < grd->nx; i++) {
				ind = j*grd->nx + i;
				grd->x[ind] = startx + i*grd->hx;
				grd->y[ind] = starty + j*grd->hy;
			}
		}
	}

	for (i = 0; i < grd->nd; i++) {
		grd->periodic[i] = 0;
	}

	return grd;
}


void grid_apply_map(grid *grd, map *mp)
{
	int i, j, k, ind;
	double *rst, *r, *s, *t;

	rst = (double*) malloc(grd->num_pts*3*sizeof(double));
	memcpy(rst, grd->xyz, grd->num_pts*3*sizeof(double));
	r = &rst[0];
	s = &rst[grd->num_pts];
	t = &rst[2*grd->num_pts];

	if (grd->nd == 3) {
		for (k = 0; k < grd->nz; k++) {
			for (j = 0; j < grd->ny; j++) {
				for (i = 0; i < grd->nx; i++) {
					ind = k*grd->nx*grd->ny + j*grd->nx + i;

					grd->x[ind] = mp->x(r[ind], s[ind], t[ind]);
					grd->y[ind] = mp->y(r[ind], s[ind], t[ind]);
					grd->z[ind] = mp->z(r[ind], s[ind], t[ind]);
				}
			}
		}
	} else {
		for (j = 0; j < grd->ny; j++) {
			for (i = 0; i < grd->nx; i++) {
				ind = j*grd->nx + i;

				grd->x[ind] = mp->x(r[ind], s[ind], 0);
				grd->y[ind] = mp->y(r[ind], s[ind], 0);
			}
		}
	}

	free(rst);
}


void grid_eval(grid *grd, func f, double *u)
{
	int i, j, k, ind;

	if (grd->nd == 3) {
		for (k = 0; k < grd->nz; k++) {
			for (j = 0; j < grd->ny; j++) {
				for (i = 0; i < grd->nx; i++) {
					ind = k*grd->nx*grd->ny + j*grd->nx + i;
					u[ind] = f(grd->x[ind], grd->y[ind], grd->z[ind]);
				}
			}
		}
	} else {
		for (j = 0; j < grd->ny; j++) {
			for (i = 0; i < grd->nx; i++) {
				ind = j*grd->nx + i;
				u[ind] = f(grd->x[ind], grd->y[ind], 0);
			}
		}
	}
}


void grid_print(grid *grd, double *u)
{
	int i, j;

	for (j = 0; j < grd->ny; j++) {
		for (i = 0; i < grd->nx; i++) {
			if (u[j*grd->nx + i] > 0)
				printf(" %5g  ", u[j*grd->nx + i]);
			else
				printf("%5g  ", u[j*grd->nx + i]);
		}
		printf("\n");
	}
	printf("\n");
}


void grid_destroy(grid *grd)
{
	free(grd->xyz);
}

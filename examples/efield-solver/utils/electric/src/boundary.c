#include <stdlib.h>

#include "boundary.h"

blist *boundary_create(grid *grd, problem *pb)
{
	int i, j;

	blist *lst = (blist*) malloc(sizeof(blist));
	if (pb->id == ELECTRODE) lst->len = 6;
	else lst->len = 4;
	if (grd->nd == 3) lst->len += 2;
	lst->v = (boundary*) malloc(lst->len*sizeof(boundary));

	lst->len = 0;
	if (grd->nd == 3) {
		for (i = 0; i < 6; i++) {
			if (i == NORTH
			    && grd->cart_coord[1] == grd->num_procs[1]-1
			    && (!grd->periodic[1])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				if (pb->id == JUMP) {
					for (j = 0; j < grd->num_pts; j++)
						lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j], grd->y[j], grd->z[j]);
				} else {
					for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;
				}

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[2] = grd->is[2];
				lst->v[lst->len].ie[2] = grd->ie[2];
				lst->v[lst->len].is[1] = grd->ie[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].norm_dir = 2;

				lst->len++;
			} else if (i == SOUTH
			           && grd->cart_coord[1] == 0
			           && (!grd->periodic[1])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				if (pb->id == JUMP) {
					for (j = 0; j < grd->num_pts; j++)
						lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j], grd->y[j], grd->z[j]);
				} else {
					for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;
				}
				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[2] = grd->is[2];
				lst->v[lst->len].ie[2] = grd->ie[2];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->is[1];
				lst->v[lst->len].norm_dir = -2;

				lst->len++;
			} else if (i == EAST
			           && grd->cart_coord[0] == grd->num_procs[0]-1
			           && (!grd->periodic[0])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));

				for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->ie[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].is[2] = grd->is[2];
				lst->v[lst->len].ie[2] = grd->ie[2];
				lst->v[lst->len].norm_dir = 1;

				lst->len++;
			} else if (i == WEST
			           && grd->cart_coord[0] == 0
			           && (!grd->periodic[0])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));

				for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->is[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].is[2] = grd->is[2];
				lst->v[lst->len].ie[2] = grd->ie[2];
				lst->v[lst->len].norm_dir = -1;

				lst->len++;
			} else if (i == FRONT
			           && grd->cart_coord[2] == grd->num_procs[2] - 1
			           && (!grd->periodic[2])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				if (pb->id == JUMP) {
					for (j = 0; j < grd->num_pts; j++)
						lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j], grd->y[j], grd->z[j]);
				} else {
					for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;
				}

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].is[2] = grd->ie[2];
				lst->v[lst->len].ie[2] = grd->ie[2];

				lst->len++;
			} else if (i == BACK
			           && grd->cart_coord[2] == 0
			           && (!grd->periodic[2])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				if (pb->id == JUMP) {
					for (j = 0; j < grd->num_pts; j++)
						lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j], grd->y[j], grd->z[j]);
				} else {
					for (j = 0; j < grd->num_pts; j++) lst->v[lst->len].dirichlet[j] = 0;
				}

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].is[2] = grd->is[2];
				lst->v[lst->len].ie[2] = grd->is[2];

				lst->len++;
			}
		}
	} else {
		for (i = 0; i < 4; i++) {
			if (i == NORTH
			    && grd->cart_coord[1] == grd->num_procs[1]-1
			    && (!grd->periodic[1])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts
				                                              *sizeof(double));
				for (j = 0; j < grd->nx; j++) {
					lst->v[lst->len].dirichlet[j] = pb->sol(
						grd->x[(grd->ny-1)*grd->nx + j],
						grd->y[(grd->ny-1)*grd->nx + j], 0);
				}

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->ie[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].norm_dir = 2;

				lst->len++;
			} else if (i == SOUTH
			           && grd->cart_coord[1] == 0
			           && (!grd->periodic[1])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));

				for (j = 0; j < grd->nx; j++)
					lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j], grd->y[j], 0);

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->is[1];
				lst->v[lst->len].norm_dir = -2;

				lst->len++;
			} else if (i == EAST
			           && grd->cart_coord[0] == grd->num_procs[0]-1
			           && (!grd->periodic[0])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));

				for (j = 0; j < grd->ny; j++)
					lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j*grd->nx + grd->nx - 1], grd->y[j*grd->nx + grd->nx - 1], 0);

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->ie[0];
				lst->v[lst->len].ie[0] = grd->ie[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].norm_dir = 1;

				lst->len++;
			} else if (i == WEST
			           && grd->cart_coord[0] == 0
			           && (!grd->periodic[0])) {
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				for (j = 0; j < grd->ny; j++)
					lst->v[lst->len].dirichlet[j] = pb->sol(grd->x[j*grd->nx], grd->y[j*grd->nx], 0);

				lst->v[lst->len].type = pb->boundary[i];

				lst->v[lst->len].is[0] = grd->is[0];
				lst->v[lst->len].ie[0] = grd->is[0];
				lst->v[lst->len].is[1] = grd->is[1];
				lst->v[lst->len].ie[1] = grd->ie[1];
				lst->v[lst->len].norm_dir = -1;

				lst->len++;
			}
		}

		if (pb->id == ELECTRODE) {
			for (i = 4; i < 6; i++) {
				lst->v[lst->len].type = DIRICHLET;
				lst->v[lst->len].dirichlet = (double*) malloc(grd->num_pts*sizeof(double));
				lst->v[lst->len].is[0] = pb->interior[i-4].is[0];
				lst->v[lst->len].ie[0] = pb->interior[i-4].ie[0];
				lst->v[lst->len].is[1] = pb->interior[i-4].is[1];
				lst->v[lst->len].ie[1] = pb->interior[i-4].ie[1];
				lst->v[lst->len].norm_dir = 1;
				for (j = 0; j < grd->num_pts; j++) {
					if (!pb->interior[i-4].positive) {
						lst->v[lst->len].dirichlet[j] = 0;
					} else {
						lst->v[lst->len].dirichlet[j] = 0;
					}
				}

				lst->len++;
			}
		}
	}

	lst->v = realloc(lst->v, lst->len*sizeof(boundary));

	return lst;
}


void boundary_destroy(blist *lst)
{
	int i;

	for (i = 0; i < lst->len; i++) {
		free(lst->v[i].dirichlet);
	}

	free(lst->v);
}

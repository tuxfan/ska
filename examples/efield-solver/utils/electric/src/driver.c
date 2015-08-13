#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "problem.h"
#include "map.h"
#include "grid.h"
#include "solver.h"
#include "option.h"


static double norm(double v[], int len)
{
	int i;

	double curr = 0;
	for (i = 0; i < len; i++) {
		if (fabs(v[i]) > curr) curr = fabs(v[i]);
	}

	return curr;
}


static void print_norm(double v[], int len)
{
	int rank;
	double n;

	double mynorm = norm(v, len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Reduce(&mynorm, &n, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0) printf("Norm: %g\n", n);
}


int main(int argc, char *argv[])
{
	PetscErrorCode ierr;
	int i;
	grid *grd;

	MPI_Init(&argc,&argv);

	option options = parse_options(argc, argv);

	if (options.problem == JUMP) {
		grd = grid_create(-1, 1, options.nx,
		                  -1, 1, options.ny,
		                  -1, 1, options.nz);
	} else {
		grd = grid_create(0, 1, options.nx,
		                  0, 1, options.ny,
		                  0, 1, options.nz);
	}
	if (options.periodic > -1) grd->periodic[options.periodic] = 1;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) print_intro(&options);

	map *mp = map_create(options.map);
	grid_apply_map(grd, mp);

	problem *pb = problem_create(options.problem, grd->nd, mp->id);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);CHKERRQ(ierr);
	ierr = solver_run(sol);CHKERRQ(ierr);

	double *u = (double*) malloc(grd->num_pts*sizeof(double));
	double *diff = (double*) malloc(grd->num_pts*sizeof(double));
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	print_norm(diff, grd->num_pts);

	free(u); free(diff);
	grid_destroy(grd); free(grd);
	map_destroy(mp); free(mp);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol);CHKERRQ(ierr); free(sol);

	if (options.eig && rank == 0) {
		system("./python/plot.py");
	}

	MPI_Finalize();

	return 0;
}

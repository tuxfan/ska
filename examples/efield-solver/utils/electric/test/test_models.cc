#include <limits.h>
#include <math.h>
#include <mpi.h>
#include "gtest/gtest.h"

extern "C" {
#include "../src/problem.h"
#include "../src/map.h"
#include "../src/grid.h"
#include "../src/solver.h"
}

namespace {
static double norm(double v[], int len)
{
	int i;

	double curr = 0;
	for (i = 0; i < len; i++) {
		if (fabs(v[i]) > curr) curr = fabs(v[i]);
	}

	return curr;
}


static double mpi_norm(double v[], int len)
{
	int rank;
	double n;

	double mynorm = norm(v, len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Reduce(&mynorm, &n, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0) return n;
	else return 0;
}

class ModelsTest : public ::testing::Test {
protected:

	ModelsTest() {
		grd = grid_create(0, 1, 301,
						  0, 1, 301,
						  0, 0, 0);
		default_tol = 1e-4;
	}

	virtual ~ModelsTest() {
		grid_destroy(grd); free(grd);
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

	grid *grd;
	map *mp;
	solver *sol;
	problem *pb;
	double *u, *diff;
	int i, ierr;
	double nrm;
	double default_tol;
};

	TEST_F(ModelsTest, UniformSine) {
		pb = problem_create(SIN, 2, 0);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, UniformMixed) {
		pb = problem_create(MIXED, 2, 0);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, StretchSine) {
		grid_destroy(grd); free(grd);
		grd = grid_create(0, 1, 601,
						  0, 1, 301,
						  0, 0, 0);
		pb = problem_create(SIN, 2, 0);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, StretchMixed) {
		grid_destroy(grd); free(grd);
		grd = grid_create(0, 1, 301,
						  0, 1, 601,
						  0, 0, 0);

		pb = problem_create(MIXED, 2, 0);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, CurlSine) {
		mp = map_create(MAP_CURL);
		grid_apply_map(grd, mp);

		pb = problem_create(SIN, 2, mp->id);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		map_destroy(mp); free(mp);
		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, SecondSine) {
		mp = map_create(MAP_SECOND);
		grid_apply_map(grd, mp);

		pb = problem_create(SIN, 2, mp->id);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		map_destroy(mp); free(mp);
		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, SecondTutorial) {
		mp = map_create(MAP_SECOND);
		grid_apply_map(grd, mp);

		pb = problem_create(MTUT, 2, mp->id);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		map_destroy(mp); free(mp);
		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

	TEST_F(ModelsTest, Diamond) {
		mp = map_create(MAP_DIAMOND);
		grid_apply_map(grd, mp);

		pb = problem_create(DIAMOND, 2, mp->id);
		sol = solver_create(grd, pb);

		ierr = solver_init(sol, grd);
		ASSERT_EQ(ierr, 0);
		ierr = solver_run(sol);
		ASSERT_EQ(ierr, 0);

		u = (double*) malloc(sizeof(double)*grd->num_pts);
		diff = (double*) malloc(sizeof(double)*grd->num_pts);
		grid_eval(grd, pb->sol, u);

		for (i = 0; i < grd->num_pts; i++)
			diff[i] = sol->state->phi[i] - u[i];

		nrm = mpi_norm(diff, grd->num_pts);
		EXPECT_LT(fabs(nrm), default_tol);

		map_destroy(mp); free(mp);
		problem_destroy(pb); free(pb);
		ierr = solver_destroy(sol); free(sol);
		ASSERT_EQ(ierr, 0);
	}

}  // namespace

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	MPI_Finalize();
	return ret;
}

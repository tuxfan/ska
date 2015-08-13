#include <math.h>
#include <check.h>
#include <mpi.h>

#include "../src/problem.h"
#include "../src/map.h"
#include "../src/grid.h"
#include "../src/solver.h"


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


START_TEST(test_uniform_sine)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);
	problem *pb = problem_create(SIN, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_uniform_mixed)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);

	problem *pb = problem_create(MIXED, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_stretch_sine)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 601,
	                        0, 1, 301,
	                        0, 0, 0);
	problem *pb = problem_create(SIN, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_stretch_mixed)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 601,
	                        0, 0, 0);

	problem *pb = problem_create(MIXED, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_curl_sine)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);

	map *mp = map_create(MAP_CURL);
	grid_apply_map(grd, mp);

	problem *pb = problem_create(SIN, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	map_destroy(mp); free(mp);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_second_sine)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);

	map *mp = map_create(MAP_SECOND);
	grid_apply_map(grd, mp);

	problem *pb = problem_create(SIN, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	map_destroy(mp); free(mp);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_second_mtut)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);

	map *mp = map_create(MAP_SECOND);
	grid_apply_map(grd, mp);

	problem *pb = problem_create(MTUT, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	map_destroy(mp); free(mp);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


START_TEST(test_diamond)
{
	int i, ierr;
	grid *grd = grid_create(0, 1, 301,
	                        0, 1, 301,
	                        0, 0, 0);

	map *mp = map_create(MAP_DIAMOND);
	grid_apply_map(grd, mp);

	problem *pb = problem_create(DIAMOND, 2);
	solver *sol = solver_create(grd, pb);

	ierr = solver_init(sol, grd);
	ck_assert_int_eq(ierr, 0);
	ierr = solver_run(sol);
	ck_assert_int_eq(ierr, 0);

	double u[grd->num_pts];
	double diff[grd->num_pts];
	grid_eval(grd, pb->sol, u);

	for (i = 0; i < grd->num_pts; i++)
		diff[i] = sol->state->phi[i] - u[i];

	double nrm = mpi_norm(diff, grd->num_pts);
	ck_assert(fabs(nrm) < 1e-4);

	grid_destroy(grd); free(grd);
	map_destroy(mp); free(mp);
	problem_destroy(pb); free(pb);
	ierr = solver_destroy(sol); free(sol);
	ck_assert_int_eq(ierr, 0);
}
END_TEST


Suite * model_suite(void)
{
	Suite *s;
	TCase *tc_uniform;
	TCase *tc_curl;
	TCase *tc_stretch;
	TCase *tc_second;
	TCase *tc_misc;

	s = suite_create("Models");
	tc_uniform = tcase_create("Uniform");
	tc_curl = tcase_create("Curl");
	tc_stretch = tcase_create("Stretched");
	tc_second = tcase_create("Second");
	tc_misc = tcase_create("Misc.");

	tcase_set_timeout(tc_uniform, 20);
	tcase_add_test(tc_uniform, test_uniform_sine);
	tcase_add_test(tc_uniform, test_uniform_mixed);
	suite_add_tcase(s, tc_uniform);

	tcase_set_timeout(tc_curl, 20);
	tcase_add_test(tc_curl, test_curl_sine);
	suite_add_tcase(s, tc_curl);

	tcase_set_timeout(tc_stretch, 20);
	tcase_add_test(tc_stretch, test_stretch_sine);
	tcase_add_test(tc_stretch, test_stretch_mixed);
	suite_add_tcase(s, tc_stretch);

	tcase_set_timeout(tc_second, 20);
	tcase_add_test(tc_second, test_second_sine);
	tcase_add_test(tc_second, test_second_mtut);
	suite_add_tcase(s, tc_second);

	tcase_set_timeout(tc_misc, 20);
	tcase_add_test(tc_misc, test_diamond);
	suite_add_tcase(s, tc_misc);

	return s;
}


int main(int argc, char *argv[])
{
	int num_failed;
	Suite *s;
	SRunner *sr;

	MPI_Init(&argc, &argv);

	s = model_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	MPI_Finalize();
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

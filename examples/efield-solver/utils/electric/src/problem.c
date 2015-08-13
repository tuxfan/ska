#include <stdlib.h>
#include <math.h>

#include "problem.h"
#include "map.h"
#include "boundary.h"


const char *problem_name[NUM_PROBLEMS] = {"Multigrid Tutorial", "Mixed Boundaries", "Sine", "Translated Sine",
                                          "Diamond Domain", "Electrode", "Jump"};
const char *problem_key[NUM_PROBLEMS] = {"tutorial", "mixed", "sin", "tsine", "diamond",
                                         "electrode", "jump"};

double electrode_rhs(double x, double y, double z)
{
	return 0;
}


double electrode_sol(double x, double y, double z)
{
	return 0;
}


double axisymmetric_rhs(double x, double y, double z)
{
	double tmpx;
	tmpx = x*cos(y);
	y = x*sin(y);
	x = tmpx;

	return -y*(-6*M_PI*pow(x, 2)*cos(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 2) - 4*pow(M_PI, 2)*pow(x, 2)*sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 3.0L/2.0L) + 3*pow(x, 2)*sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 5.0L/2.0L) + 2*M_PI*cos(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/(pow(x, 2) + pow(y, 2)) - sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 3.0L/2.0L)) - y*(-6*M_PI*pow(y, 2)*cos(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 2) - 4*pow(M_PI, 2)*pow(y, 2)*sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 3.0L/2.0L) + 3*pow(y, 2)*sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 5.0L/2.0L) + 6*M_PI*cos(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/(pow(x, 2) + pow(y, 2)) - 3*sin(2*M_PI*sqrt(pow(x, 2) + pow(y, 2)))/pow(pow(x, 2) + pow(y, 2), 3.0L/2.0L));
}


double axisymmetric_sol(double x, double y, double z)
{
	return sin(2*M_PI*x)*sin(y);
}


double mtut_rhs(double x, double y, double z)
{
	return 2*((1-(6*(x*x)))*(y*y)*(1-(y*y)) + (1 - (6*(y*y)))*(x*x)*(1-(x*x)));
}


double mtut_sol(double x, double y, double z)
{
	return (x*x - x*x*x*x)*(y*y*y*y - y*y);
}


double sin_rhs(double x, double y, double z)
{
	return 8*(M_PI*M_PI)*sin(2*M_PI*x)*sin(2*M_PI*y);
}


double sin_sol(double x, double y, double z)
{
	return sin(2*M_PI*x)*sin(2*M_PI*y);
}


double tsin_rhs(double x, double y, double z)
{
	return 8*(M_PI*M_PI)*sin(2*M_PI*x + 1.4)*sin(2*M_PI*y+1);
}


double tsin_sol(double x, double y, double z)
{
	return sin(2*M_PI*x+1.4)*sin(2*M_PI*y+1);
}


double polar_tsin_rhs(double x, double y, double z)
{
	return M_PI*M_PI*x*x*sin(M_PI*(sqrt(x*x + y*y) - 0.5))*sin(atan2(y, x) + 1.4)/(x*x + y*y) + x*x*sin(M_PI*(sqrt(x*x + y*y) - 0.5))*sin(atan2(y, x) + 1.4)/pow((x*x + y*y),2) + M_PI*x*x*sin(atan2(y, x) + 1.4)*cos(M_PI*(sqrt(x*x + y*y) - 0.5))/pow((x*x + y*y),(3./2)) + M_PI*M_PI*y*y*sin(M_PI*(sqrt(x*x + y*y) - 0.5))*sin(atan2(y, x) + 1.4)/(x*x + y*y) + y*y*sin(M_PI*(sqrt(x*x + y*y) - 0.5))*sin(atan2(y, x) + 1.4)/pow((x*x + y*y),2) + M_PI*y*y*sin(atan2(y, x) + 1.4)*cos(M_PI*(sqrt(x*x + y*y) - 0.5))/pow((x*x + y*y),(3./2)) - 2*M_PI*sin(atan2(y, x) + 1.4)*cos(M_PI*(sqrt(x*x + y*y) - 0.5))/sqrt(x*x + y*y);
}


double polar_tsin_sol(double x, double y, double z)
{
	double tmpx = sqrt(x*x + y*y) - .5;
	y = atan2(y,x) / (2*M_PI);
	x = tmpx;

	return sin(M_PI*x)*sin(2*M_PI*y + 1.4);
}


double polar_sin_rhs(double x, double y, double z)
{
	return -y*(-6*M_PI*x*x*cos(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),2) - 4*(M_PI*M_PI)*x*x*sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(3./2)) + 3*x*x*sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(5./2)) + 2*M_PI*cos(M_PI*(2*sqrt(x*x + y*y) - 1.0))/(x*x + y*y) - sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(3./2))) - y*(-6*M_PI*y*y*cos(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),2) - 4*(M_PI*M_PI)*y*y*sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(3./2)) + 3*y*y*sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(5./2)) + 6*M_PI*cos(M_PI*(2*sqrt(x*x + y*y) - 1.0))/(x*x + y*y) - 3*sin(M_PI*(2*sqrt(x*x + y*y) - 1.0))/pow((x*x + y*y),(3./2)));
}


double polar_sin_sol(double x, double y, double z)
{
	double tmpx = sqrt(x*x + y*y) - .5;
	y = atan2(y,x) / (2*M_PI);
	x = tmpx;

	return sin(2*M_PI*x)*sin(2*M_PI*y);
}


double sin_rhs_3d(double x, double y, double z)
{
	return 12*(M_PI*M_PI)*sin(2*M_PI*x)*sin(2*M_PI*y)*sin(2*M_PI*z);
}


double sin_sol_3d(double x, double y, double z)
{
	return sin(2*M_PI*x)*sin(2*M_PI*y)*sin(2*M_PI*z);
}


double mixed_rhs(double x, double y, double z)
{
	//return 4*(M_PI*M_PI)*(cos(2*M_PI*x) - 1)*cos(2*M_PI*y) + 4*(M_PI*M_PI)*(cos(2*M_PI*y) - 1)*cos(2*M_PI*x);
	return 1.25*(M_PI*M_PI)*sin(0.5*M_PI*y)*cos(M_PI*x);
}


double mixed_sol(double x, double y, double z)
{
	//return (1- cos(2*M_PI*x))*(1- cos(2*M_PI*y));
	return cos(M_PI*x)*sin(.5*M_PI*y);
}

double mixed_rhs_3d(double x, double y, double z)
{
	return -4*(M_PI*M_PI)*(cos(2*M_PI*x) - 1)*(cos(2*M_PI*y) - 1)*cos(2*M_PI*z) - 4*(M_PI*M_PI)*(cos(2*M_PI*x) - 1)*(cos(2*M_PI*z) - 1)*cos(2*M_PI*y) - 4*(M_PI*M_PI)*(cos(2*M_PI*y) - 1)*(cos(2*M_PI*z) - 1)*cos(2*M_PI*x);
}


double mixed_sol_3d(double x, double y, double z)
{
	return (1- cos(2*M_PI*x))*(1- cos(2*M_PI*y)) * (1-cos(2*M_PI*z));
}


double diamond_sol(double x, double y, double z)
{
	return sin(M_PI*(x + y + 1))*sin(M_PI*(y - x + 1));
}


double diamond_rhs(double x, double y, double z)
{
	return -2*(M_PI*M_PI)*(-sin((M_PI)*(-x + y + 1))*sin(M_PI*(x + y + 1)) + cos(M_PI*(-x + y + 1))*cos(M_PI*(x + y + 1))) + 2*(M_PI*M_PI)*(sin(M_PI*(-x + y + 1))*sin(M_PI*(x + y + 1)) + cos(M_PI*(-x + y + 1))*cos(M_PI*(x + y + 1)));

}


double jump_sol(double x, double y, double z)
{
	if (x <= 0)
		return (2*(x*x) + 2*x);
	else
		return (-4*(x*x) + 4*x);
}


double jump_rhs(double x, double y, double z)
{
	if (x <= 0)
		return -16;
	else
		return 16;
}


problem *problem_create(problem_id id, int nd, int map_id)
{
	problem *pb = (problem*) malloc(sizeof(problem));
	pb->id = id;
	pb->nd = nd;

	switch(id) {
	case (ELECTRODE):
		pb->rhs = &electrode_rhs;
		pb->sol = &electrode_sol;
		pb->interior[0].positive = 0;
		pb->interior[1].positive = 1;
		pb->interior[0].is[0] = 3;
		pb->interior[0].is[1] = 3;
		pb->interior[0].ie[0] = 7;
		pb->interior[0].ie[1] = 7;
		pb->interior[1].is[0] = 3;
		pb->interior[1].is[1] = 3;
		pb->interior[1].ie[0] = 7;
		pb->interior[1].ie[1] = 7;
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		break;
	case (MTUT):
		pb->rhs = &mtut_rhs;
		pb->sol = &mtut_sol;
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		break;
	case (SIN):
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		if (nd == 3) {
			pb->rhs = &sin_rhs_3d;
			pb->sol = &sin_sol_3d;
			pb->boundary[FRONT] =  DIRICHLET;
			pb->boundary[BACK] =  DIRICHLET;
		} else {
			if (map_id == MAP_POLAR) {
				pb->rhs = &polar_sin_rhs;
				pb->sol = &polar_sin_sol;
			} else {
				pb->rhs = &sin_rhs;
				pb->sol = &sin_sol;
			}
		}
		break;
	case(TSINE):
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		if (map_id == MAP_POLAR) {
			pb->sol = &polar_tsin_sol;
			pb->rhs = &polar_tsin_rhs;
		} else {
			pb->sol = &tsin_sol;
			pb->rhs = &tsin_rhs;
		}
		break;
	case (MIXED):
		if (nd == 2) {
			pb->rhs = &mixed_rhs;
			pb->sol = &mixed_sol;
			pb->boundary[NORTH] = NEUMANN;
			pb->boundary[SOUTH] = DIRICHLET;
			pb->boundary[EAST] =  NEUMANN;
			pb->boundary[WEST] =  NEUMANN;
		} else {
			pb->rhs = &mixed_rhs_3d;
			pb->sol = &mixed_sol_3d;
			pb->boundary[NORTH] = NEUMANN;
			pb->boundary[SOUTH] = DIRICHLET;
			pb->boundary[EAST] =  NEUMANN;
			pb->boundary[WEST] =  DIRICHLET;
			pb->boundary[FRONT] = NEUMANN;
			pb->boundary[BACK] = DIRICHLET;
		}

		break;
	case (DIAMOND):
		pb->rhs = &diamond_rhs;
		pb->sol = &diamond_sol;
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		break;
	case (JUMP):
		pb->rhs = &jump_rhs;
		pb->sol = &jump_sol;
		pb->boundary[NORTH] = DIRICHLET;
		pb->boundary[SOUTH] = DIRICHLET;
		pb->boundary[EAST] =  DIRICHLET;
		pb->boundary[WEST] =  DIRICHLET;
		if (nd == 3) {
			pb->boundary[FRONT] =  DIRICHLET;
			pb->boundary[BACK] =  DIRICHLET;
		}
		break;
	default:
		pb->rhs = &mtut_rhs;
		pb->sol = &mtut_sol;
		break;
	}

	return pb;
}


void problem_destroy(problem *pb)
{
}

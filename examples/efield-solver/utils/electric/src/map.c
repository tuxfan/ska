#include <stdlib.h>
#include <math.h>

#include "map.h"


const char *map_name[NUM_MAPS] = {"Identity", "Stretched", "Constant Second Derivative",
                                  "Divergence Free", "Curl Free", "Diamond Domain", "Disk"};
const char *map_key[NUM_MAPS] = {"identity", "stretched", "second", "curl", "div", "diamond", "disk"};


double map_identity_x(double r, double s, double t)
{
	return r;
}


double map_identity_y(double r, double s, double t)
{
	return s;
}


double map_identity_z(double r, double s, double t)
{
	return t;
}


double map_polar_x(double r, double s, double t)
{
	return (r+.5) * cos(2*M_PI*s);
}


double map_polar_y(double r, double s, double t)
{
	return (r+.5) * sin(2*M_PI*s);
}


double map_second_x(double r, double s, double t)
{
	//return xi;//*xi;
	return sin(.5 * M_PI * r);
	//return 3*xi - 1;
}


double map_second_y(double r, double s, double t)
{
	//return sqrt(eta);
	//return eta;//*eta;
	//return sin(.5 * M_PI * eta);
	return s*s;
	//return 3*eta-1;
}


double map_second_z(double r, double s, double t)
{
	return t*t;
}


double map_curl_x(double r, double s, double t)
{
	return -s + 1;
}


double map_curl_y(double r, double s, double t)
{
	return r;
}


double map_curl_y_3d(double r, double s, double t)
{
	return -r + 1;
}


double map_curl_z_3d(double r, double s, double t)
{
	return -t + 1;
}


double map_div_x(double r, double s, double t)
{
	return 3*r - 1;
}


double map_div_y(double r, double s, double t)
{
	return 3*s - 1;
}


double map_diamond_x(double r, double s, double t)
{
	return r - s;
}


double map_diamond_y(double r, double s, double t)
{
	return r + s - 1;
}


map *map_create(map_id mid)
{
	map *ret = (map*) malloc(sizeof(map));

	ret->id = mid;
	switch (mid) {
	case(MAP_IDENTITY):
		ret->x = &map_identity_x;
		ret->y = &map_identity_y;
		ret->z = &map_identity_z;
		break;
	case(MAP_POLAR):
		ret->x = &map_polar_x;
		ret->y = &map_polar_y;
		break;
	case(MAP_SECOND):
		ret->x = &map_second_x;
		ret->y = &map_second_y;
		ret->z = &map_second_z;
		break;
	case(MAP_CURL):
		ret->x = &map_curl_x;
		ret->y = &map_curl_y;
		break;
	case(MAP_DIV):
		ret->x = &map_div_x;
		ret->y = &map_div_y;
		break;
	case(MAP_DIAMOND):
		ret->x = &map_diamond_x;
		ret->y = &map_diamond_y;
		break;
	default:
		ret->x = &map_identity_x;
		ret->y = &map_identity_y;
		break;
	}

	return ret;
}


void map_destroy(map *mp)
{
}

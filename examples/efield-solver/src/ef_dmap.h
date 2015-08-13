#ifndef EF_DMAP_H
#define EF_DMAP_H

#include "petscsys.h"


typedef struct ef_dmap {
	int xs, ys, zs, xm, ym, zm;
	int nd;
	int stride[3];
	PetscErrorCode (*get_2d)(struct ef_dmap *mp, double *src, double ***array);
	PetscErrorCode (*restore_2d)(struct ef_dmap *mp, double ***array);
	PetscErrorCode (*get_3d)(struct ef_dmap *mp, double *src, double ****array);
	PetscErrorCode (*restore_3d)(struct ef_dmap *mp, double ****array);
} ef_dmap;


ef_dmap *ef_dmap_create_3d(int xs, int ys, int zs, int xm, int ym, int zm, int stride[]);


ef_dmap *ef_dmap_create_2d(int xs, int ys, int xm, int ym, int stride[]);


PetscErrorCode ef_dmap_get(ef_dmap *mp, double *src, void *array);


PetscErrorCode ef_dmap_restore(ef_dmap *mp, void *array);


PetscErrorCode ef_dmap_destroy(ef_dmap *mp);


#endif

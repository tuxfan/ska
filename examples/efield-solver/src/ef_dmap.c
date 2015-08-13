#include <stdlib.h>

#include "ef_dmap.h"


PetscErrorCode plascom_get_3d(ef_dmap *idx, double *src, double ****dest)
{
	PetscErrorCode ierr;
	double **b;
	int i, j;
	/*
	  m => zm
	  n => ym
	  p => xm
	  mstart => zs
	  nstart => ys
	  pstart => xs
	*/
	ierr = PetscMalloc1(idx->stride[2]*sizeof(double**) + idx->stride[2]*idx->stride[1], dest);CHKERRQ(ierr);

	b = (double**)((*dest) + idx->stride[2]);
	for (i = 0; i < idx->stride[2]; i++) (*dest)[i] = b + i*idx->stride[1] - idx->ys;
	for (i = 0; i < idx->stride[2]; i++) {
		for (j = 0; j < idx->stride[1]; j++) {
			b[i*idx->stride[1] + j] = src + i*idx->stride[0]*idx->stride[1] + j*idx->stride[0] - idx->xs;
		}
	}

	*dest -= idx->zs;

	return 0;
}


PetscErrorCode plascom_restore_3d(ef_dmap *idx, double ****arr)
{
	PetscErrorCode ierr;
	void *dummy;

	dummy = (void*)(*arr + idx->zs);
	ierr = PetscFree(dummy);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode plascom_get_2d(ef_dmap *idx, double *src, double ***dest)
{
	PetscErrorCode ierr;
	int i;

	ierr = PetscMalloc1(idx->stride[1], dest);CHKERRQ(ierr);

	for (i = 0; i < idx->stride[1]; i++) {
		(*dest)[i] = src + i*idx->stride[0] - idx->xs;
	}
	*dest -= idx->ys;

	return 0;
}


PetscErrorCode plascom_restore_2d(ef_dmap *idx, double ***arr)
{
	PetscErrorCode ierr;
	void *dummy;

	dummy = (void*)(*arr + idx->ys);
	ierr = PetscFree(dummy);CHKERRQ(ierr);

	return 0;
}


ef_dmap *ef_dmap_create_3d(int xs, int ys, int zs, int xm, int ym, int zm, int stride[])
{
	ef_dmap *mp = (ef_dmap*) malloc(sizeof(ef_dmap));

	mp->xs = xs; mp->ys = ys; mp->zs = zs;
	mp->xm = xm; mp->ym = ym; mp->zm = zm;
	mp->nd = 3;
	mp->stride[0] = stride[0];
	mp->stride[1] = stride[1];
	mp->stride[2] = stride[2];

	mp->get_3d = &plascom_get_3d;
	mp->restore_3d = &plascom_restore_3d;

	return mp;
}


ef_dmap *ef_dmap_create_2d(int xs, int ys, int xm, int ym, int stride[])
{
	ef_dmap *mp = (ef_dmap*) malloc(sizeof(ef_dmap));
	mp->xs = xs; mp->ys = ys;
	mp->xm = xm; mp->ym = ym;
	mp->nd = 2;
	mp->stride[0] = stride[0];
	mp->stride[1] = stride[1];

	mp->get_2d = &plascom_get_2d;
	mp->restore_2d = &plascom_restore_2d;

	return mp;
}


PetscErrorCode ef_dmap_get(ef_dmap *mp, double *src, void *array)
{
	PetscErrorCode ierr;

	if (mp->nd == 3) {
		ierr = mp->get_3d(mp, src, (double ****) array);CHKERRQ(ierr);
	} else {
		ierr = mp->get_2d(mp, src, (double ***) array);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_dmap_restore(ef_dmap *mp, void *array)
{
	PetscErrorCode ierr;

	if (mp->nd == 3) {
		ierr = mp->restore_3d(mp, (double ****) array);CHKERRQ(ierr);
	} else {
		ierr = mp->restore_2d(mp, (double ***) array);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_dmap_destroy(ef_dmap *mp)
{
	return 0;
}


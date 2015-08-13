#include <stdlib.h>

#include "ef_stencil.h"


PetscErrorCode ef_stencil_create(ef_stencil **sten, int len, int off)
{
	ef_stencil *st;

	st = (ef_stencil*) malloc(sizeof(ef_stencil));
	st->v = (double*) malloc(len*sizeof(double));
	st->v += off;

	st->len = len;
	st->off = off;

	*sten = st;
	return 0;
}


PetscErrorCode ef_stencil_destroy(ef_stencil *sten)
{
	double *dummy;

	dummy = sten->v - sten->off;
	free(dummy);

	return 0;
}

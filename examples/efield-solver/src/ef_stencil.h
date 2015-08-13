#ifndef EF_STENCIL_H
#define EF_STENCIL_H

#include <petscsys.h>


typedef struct {
	double *v;
	int len;
	int off;
} ef_stencil;


PetscErrorCode ef_stencil_create(ef_stencil **sten, int len, int off);


PetscErrorCode ef_stencil_destroy(ef_stencil *sten);


#endif

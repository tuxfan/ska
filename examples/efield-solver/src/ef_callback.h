#ifndef EF_CALLBACK_H
#define EF_CALLBACK_H

#include <petscdmda.h>
#include <petscmat.h>


typedef struct {
	PetscErrorCode (*restrct)(DM dmf, Mat rmat, Vec rscale, Mat inject, DM dmc, void *user_ctx);
	PetscErrorCode (*coarsen)(DM dmf, DM dmc, void *user_ctx);
	PetscErrorCode (*guess)(KSP ksp, Vec x, void *user_ctx);
	PetscErrorCode (*rhs)(KSP ksp, Vec b, void *user_ctx);
	PetscErrorCode (*matrix)(KSP ksp, Mat A, Mat B, void *user_ctx);
	PetscErrorCode (*presolve)(KSP ksp, Vec b, Vec x, void *ctx);
} ef_callback;


PetscErrorCode ef_callback_create(ef_callback **efc);


#endif

#ifndef EF_METRIC_H
#define EF_METRIC_H

#include <petscdmda.h>

#include "ef_fd.h"

typedef struct {
	Vec jac[9];
	Vec jac2[18];
	int t2map[2][3];
	int t3map[3][6];
} ef_metric;


PetscErrorCode ef_metric_create(ef_metric**,DM,ef_fd*);


PetscErrorCode ef_metric_destroy(ef_metric*);


#endif

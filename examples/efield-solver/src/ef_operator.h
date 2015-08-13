#ifndef EF_OPERATOR_H
#define EF_OPERATOR_H

#include <petscdmda.h>

#include "ef_metric.h"
#include "ef_level.h"
#include "ef_fd.h"


typedef struct ef_operator_{
	PetscErrorCode (*assemble)(struct ef_operator_ *op, Mat A, DM da);
	ef_level *levels;
	ef_fd *fd;
	int axisymmetric;
} ef_operator;


PetscErrorCode ef_operator_create(ef_operator **efop, ef_level *levels, ef_fd *fd, int nd);


PetscErrorCode ef_operator_assemble(ef_operator *op, Mat A, DM da);


PetscErrorCode ef_operator_destroy(ef_operator*);


#endif

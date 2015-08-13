#ifndef EF_LEVEL_H
#define EF_LEVEL_H

#include <petscvec.h>

#include "ef_metric.h"

/**
 * Data structure containing data
 * needed on each MG level
 */
typedef struct {
	Vec eps;    /** Permittivity */
	Vec ag;     /** Matrix * g */
	Vec g;      /** Dirichlet boundary conditions */
	Vec gcomp;  /** Used to zero Dirichlet before scaling */
	Vec scale;  /** Future hook for system scaling */
	Vec nscale; /** Neumann scaling for symmetry */
	ef_metric *metric;
} ef_level;


#endif

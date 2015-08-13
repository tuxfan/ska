#ifndef EF_FD_H
#define EF_FD_H

#include <petscsys.h>

#include "ef_stencil.h"

typedef const char* ef_fd_type;

#define EF_FD_STANDARD_O2 "standard second order"

typedef struct {
	ef_stencil **first;
	ef_stencil **second;
	ef_fd_type type;
} ef_fd;


PetscErrorCode ef_fd_create(ef_fd **fd, ef_fd_type type);


PetscErrorCode ef_fd_destroy(ef_fd *fd);


#endif

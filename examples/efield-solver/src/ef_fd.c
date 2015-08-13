#include <stdlib.h>

#include "ef_fd.h"


static PetscErrorCode set_second_order(ef_fd *fd)
{
	PetscErrorCode ierr;

	ierr = ef_stencil_create(&fd->first[-1], 3, 0);CHKERRQ(ierr);
	ierr = ef_stencil_create(&fd->second[-1], 4, 0);CHKERRQ(ierr);

	ierr = ef_stencil_create(&fd->first[0], 3, 1);CHKERRQ(ierr);
	ierr = ef_stencil_create(&fd->second[0], 3, 1);CHKERRQ(ierr);

	ierr = ef_stencil_create(&fd->first[1], 3, 2);CHKERRQ(ierr);
	ierr = ef_stencil_create(&fd->second[1], 4, 3);CHKERRQ(ierr);

	fd->first[0]->v[-1] = -0.5;
	fd->first[0]->v[0] = 0;
	fd->first[0]->v[1] = 0.5;

	fd->first[-1]->v[0] = -3*.5;
	fd->first[-1]->v[1] = 4*.5;
	fd->first[-1]->v[2] = -.5;

	fd->first[1]->v[0] = 3*.5;
	fd->first[1]->v[-1] = -4*.5;
	fd->first[1]->v[-2] = .5;

	fd->second[0]->v[-1] = 1;
	fd->second[0]->v[0] = -2;
	fd->second[0]->v[1] = 1;

	fd->second[-1]->v[0] = 2;
	fd->second[-1]->v[1] = -5;
	fd->second[-1]->v[2] = 4;
	fd->second[-1]->v[3] = -1;

	fd->second[1]->v[0] = 2;
	fd->second[1]->v[-1] = -5;
	fd->second[1]->v[-2] = 4;
	fd->second[1]->v[-3] = -1;

	return 0;
}


PetscErrorCode ef_fd_create(ef_fd **effd, ef_fd_type type)
{
	PetscErrorCode ierr;

	ef_fd *fd = (ef_fd*) malloc(sizeof(ef_fd));

	fd->first = (ef_stencil**) malloc(3*sizeof(ef_stencil*));
	fd->second = (ef_stencil**) malloc(3*sizeof(ef_stencil*));
	fd->first++;
	fd->second++;

	if (strcmp(type, EF_FD_STANDARD_O2) == 0) {
		ierr = set_second_order(fd);CHKERRQ(ierr);
	}

	*effd = fd;
	return 0;
}


PetscErrorCode ef_fd_destroy(ef_fd *fd)
{
	PetscErrorCode ierr;
	int i;

	fd->first--;
	fd->second--;

	for(i = 0; i < 3; i++) {
		ierr = ef_stencil_destroy(fd->first[i]);CHKERRQ(ierr);
		free(fd->first[i]);
		ierr = ef_stencil_destroy(fd->second[i]);CHKERRQ(ierr);
		free(fd->second[i]);
	}

	free(fd->first);
	free(fd->second);

	return 0;
}

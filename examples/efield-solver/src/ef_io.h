#ifndef EF_IO_H
#define EF_IO_H

#include <petscdmda.h>

PetscErrorCode ef_io_vtkwrite(DM da, Vec v, char *name, int grid_id, int ts);
PetscErrorCode ef_io_eigwrite(PetscReal r[], PetscReal c[], PetscInt neig);
PetscErrorCode ef_io_print(MPI_Comm comm, char *msg);

#endif

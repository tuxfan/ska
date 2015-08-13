#include "ef_io.h"

#include <stdlib.h>

PetscErrorCode ef_io_vtkwrite(DM da, Vec v, char *name, int grid_id, int ts)
{
	PetscErrorCode ierr;
	PetscViewer drawer;
	MPI_Comm comm;
	char fname[128];
	char gname[64];

	ierr = PetscObjectGetComm((PetscObject) v, &comm);CHKERRQ(ierr);

	if (grid_id)
		snprintf(gname, sizeof gname, "grid_%d-", grid_id);
	else
		gname[0] = '\0';

	if (ts)
		snprintf(fname, sizeof fname, "ef-%s%s-%d.vts", gname, name, ts);
	else
		snprintf(fname, sizeof fname, "ef-%s%s.vts", gname, name);

	ierr = PetscViewerVTKOpen(comm,fname,FILE_MODE_WRITE,&drawer);CHKERRQ(ierr);
	ierr = DMView(da, drawer);CHKERRQ(ierr);
	ierr = VecView(v, drawer);CHKERRQ(ierr);
	ierr = PetscViewerDestroy(&drawer);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode ef_io_eigwrite(PetscReal r[], PetscReal c[], PetscInt neig)
{
	PetscErrorCode ierr;
	int i;
	PetscViewer vw;

	ierr = PetscViewerASCIIOpen(MPI_COMM_WORLD, "data/eigs.txt", &vw);CHKERRQ(ierr);

	for (i = 0; i < neig; i++) {
		if (c[i] >= 0) {
			ierr = PetscViewerASCIIPrintf(vw, "%g+%gj\n", r[i], c[i]);CHKERRQ(ierr);
		} else {
			ierr = PetscViewerASCIIPrintf(vw, "%g%gj\n", r[i], c[i]);CHKERRQ(ierr);
		}
	}

	ierr = PetscViewerDestroy(&vw);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode ef_io_print(MPI_Comm comm, char *msg)
{
	PetscErrorCode ierr;

	ierr = PetscPrintf(comm, "PlasComCM: %s\n", msg);CHKERRQ(ierr);

	return 0;
}

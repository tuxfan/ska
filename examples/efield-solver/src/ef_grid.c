#include "stdio.h"

#include "ef_grid.h"


static PetscErrorCode setup_2d(ef_grid *grid, DM *dm, MPI_Comm comm, int nGlobal[], int nProcs[], int nLocal[], int cartCoord[], int periodic[])
{
	PetscErrorCode ierr;
	int i;
	int *lnPerProcx, *lnPerProcy,
		*rnPerProcx, *rnPerProcy;
	PetscInt *nPerProcx, *nPerProcy;
	DMBoundaryType btx, bty;

	lnPerProcx = (int*) malloc((nProcs[0])*sizeof(int));
	lnPerProcy = (int*) malloc((nProcs[1])*sizeof(int));
	rnPerProcx = (int*) malloc((nProcs[0])*sizeof(int));
	rnPerProcy = (int*) malloc((nProcs[1])*sizeof(int));
	nPerProcx = (PetscInt*)malloc((nProcs[0])*sizeof(PetscInt));
	nPerProcy = (PetscInt*)malloc((nProcs[1])*sizeof(PetscInt));

	for (i=0;i<(nProcs[0]);i++) lnPerProcx[i] = 0;
	for (i=0;i<(nProcs[1]);i++) lnPerProcy[i] = 0;

	if ((cartCoord[0]) == 0)
		lnPerProcy[cartCoord[1]] = nLocal[1];
	if ((cartCoord[1]) == 0)
		lnPerProcx[cartCoord[0]] = nLocal[0];

	ierr = MPI_Allreduce(lnPerProcx, rnPerProcx, nProcs[0], MPI_INT, MPI_SUM, comm);CHKERRQ(ierr);
	ierr = MPI_Allreduce(lnPerProcy, rnPerProcy, nProcs[1], MPI_INT, MPI_SUM, comm);CHKERRQ(ierr);

	for (i=0;i<(nProcs[0]); i++)
		nPerProcx[i] = rnPerProcx[i];
	for (i=0;i<(nProcs[1]);i++)
		nPerProcy[i] = rnPerProcy[i];

	if (periodic[0]) btx = DM_BOUNDARY_PERIODIC;
	else btx = DM_BOUNDARY_GHOSTED;
	if (periodic[1]) bty = DM_BOUNDARY_PERIODIC;
	else bty = DM_BOUNDARY_GHOSTED;

	ierr = DMDACreate2d(comm, btx, bty, DMDA_STENCIL_BOX,
	                    nGlobal[0], nGlobal[1], nProcs[0], nProcs[1], 1, 2,
	                    nPerProcx, nPerProcy, dm);CHKERRQ(ierr);

	ierr = DMDASetUniformCoordinates(*dm, 0, 1, 0, 1, 0.0, 0.0);CHKERRQ(ierr);

	// BEGIN DEBUG
	/* int rank, xs, ys, xm, ym; */
	/* ierr = DMDAGetCorners(*dm, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr); */
	/* MPI_Comm_rank(old_comm, &rank); */
	/* printf("[%d] (%d,%d) -> (%d,%d)\n", rank, */
	/*         xs+1, ys+1, xs + xm, ys + ym); */
	// END DEBUG

	free(lnPerProcx); free(lnPerProcy);
	free(rnPerProcx); free(rnPerProcy);
	free(nPerProcx);  free(nPerProcy);

	return 0;
}


static PetscErrorCode setup_3d(ef_grid *grid, DM *dm, MPI_Comm comm, int nGlobal[], int nProcs[], int nLocal[], int cartCoord[], int periodic[])
{
	PetscErrorCode ierr;
	int i;
	int *lnPerProcx, *lnPerProcy, *lnPerProcz,
		*rnPerProcx, *rnPerProcy, *rnPerProcz;
	PetscInt *nPerProcx, *nPerProcy, *nPerProcz;
	DMBoundaryType btx, bty, btz;

	lnPerProcx = (int*) malloc((nProcs[0])*sizeof(int));
	lnPerProcy = (int*) malloc((nProcs[1])*sizeof(int));
	lnPerProcz = (int*) malloc((nProcs[2])*sizeof(int));
	rnPerProcx = (int*) malloc((nProcs[0])*sizeof(int));
	rnPerProcy = (int*) malloc((nProcs[1])*sizeof(int));
	rnPerProcz = (int*) malloc((nProcs[2])*sizeof(int));
	nPerProcx = (PetscInt*)malloc((nProcs[0])*sizeof(PetscInt));
	nPerProcy = (PetscInt*)malloc((nProcs[1])*sizeof(PetscInt));
	nPerProcz = (PetscInt*)malloc((nProcs[2])*sizeof(PetscInt));

	for (i=0;i<(nProcs[0]);i++) lnPerProcx[i] = 0;
	for (i=0;i<(nProcs[1]);i++) lnPerProcy[i] = 0;
	for (i=0;i<(nProcs[2]);i++) lnPerProcz[i] = 0;

	if (cartCoord[0] == 0 && cartCoord[1] == 0) {
		lnPerProcz[cartCoord[2]] = nLocal[2];
	}
	if (cartCoord[1] == 0 && cartCoord[2] == 0) {
		lnPerProcx[cartCoord[0]] = nLocal[0];
	}
	if (cartCoord[0] == 0 && cartCoord[2] == 0) {
		lnPerProcy[cartCoord[1]] = nLocal[1];
	}

	ierr = MPI_Allreduce(lnPerProcx, rnPerProcx, nProcs[0], MPI_INT, MPI_SUM, comm);CHKERRQ(ierr);
	ierr = MPI_Allreduce(lnPerProcy, rnPerProcy, nProcs[1], MPI_INT, MPI_SUM, comm);CHKERRQ(ierr);
	ierr = MPI_Allreduce(lnPerProcz, rnPerProcz, nProcs[2], MPI_INT, MPI_SUM, comm);CHKERRQ(ierr);

	for (i=0;i<(nProcs[0]); i++)
		nPerProcx[i] = rnPerProcx[i];
	for (i=0;i<(nProcs[1]);i++)
		nPerProcy[i] = rnPerProcy[i];
	for (i=0;i<(nProcs[2]);i++)
		nPerProcz[i] = rnPerProcz[i];

	if (periodic[0]) btx = DM_BOUNDARY_PERIODIC;
	else btx = DM_BOUNDARY_GHOSTED;
	if (periodic[1]) bty = DM_BOUNDARY_PERIODIC;
	else bty = DM_BOUNDARY_GHOSTED;
	if (periodic[2]) btz = DM_BOUNDARY_PERIODIC;
	else btz = DM_BOUNDARY_GHOSTED;

	ierr = DMDACreate3d(comm,
	                    btx, bty, btz,
	                    DMDA_STENCIL_BOX,
	                    nGlobal[0], nGlobal[1], nGlobal[2],
	                    nProcs[0], nProcs[1], nProcs[2],
	                    1, 2,
	                    nPerProcx, nPerProcy, nPerProcz, dm);CHKERRQ(ierr);

	ierr = DMDASetUniformCoordinates(*dm, 0, 1, 0, 1, 0, 1);CHKERRQ(ierr);

	// BEGIN DEBUG
	/* int xs, ys, zs, xm, ym, zm; */
	/* ierr = DMDAGetCorners(*dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr); */
	/* printf("(%d,%d,%d) -> (%d,%d,%d)\n", */
	/*        xs, ys, zs, */
	/*        xs + xm, ys + ym, zs + zm); */
	/* MPI_Barrier(comm); */
	/* SETERRQ(PETSC_COMM_WORLD, PETSC_ERR_SUP,"HERE"); */
	// END DEBUG

	free(lnPerProcx);
	free(lnPerProcy);
	free(lnPerProcz);
	free(rnPerProcx);
	free(rnPerProcy);
	free(rnPerProcz);
	free(nPerProcx);
	free(nPerProcy);
	free(nPerProcz);

	return 0;
}


PetscErrorCode ef_grid_setup(ef_grid *grid, DM *dm, MPI_Comm *comm, int nGlobal[], int nProcs[], int nLocal[], int cartCoord[], int periodic[], int nd)
{
	PetscErrorCode ierr;

	grid->nd = nd;

	if (nd == 2) {
		// Align cart to DMDA ordering
		MPI_Comm new_comm;
		int new_rank;

		new_rank = cartCoord[1]*nProcs[0] + cartCoord[0];
		MPI_Comm_split(*comm, 1, new_rank, &new_comm);
		*comm = new_comm;

		ierr = setup_2d(grid, dm, *comm, nGlobal, nProcs, nLocal, cartCoord, periodic);CHKERRQ(ierr);
	} else if (nd == 3) {
		// Align cart to DMDA ordering
		MPI_Comm new_comm;
		int new_rank;

		new_rank = cartCoord[2]*nProcs[0]*nProcs[1] + cartCoord[1]*nProcs[0] + cartCoord[0];
		MPI_Comm_split(*comm, 1, new_rank, &new_comm);
		*comm = new_comm;

		ierr = setup_3d(grid, dm, *comm, nGlobal, nProcs, nLocal, cartCoord, periodic);CHKERRQ(ierr);
	} else {
		SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_SUP, "Unsupported dimmension: %d", nd);
	}

	return 0;
}

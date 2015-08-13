#include <stdlib.h>

#include "ef_solver.h"
#include "ef_io.h"


PetscErrorCode efs_create(efs **slvctx, MPI_Comm comm)
{
	efs *slv = (efs*) malloc(sizeof(efs));

	slv->comm = comm;

	slv->options.matfree = 0;
	slv->options.galerkin = 0;
	slv->options.levels = 1;
	slv->options.axisymmetric = 0;

	*slvctx = slv;

	return 0;
}


PetscErrorCode efs_setup(efs *slv, int offset[], int stride[])
{
	PetscErrorCode ierr;
	PetscInt xs, ys, zs, xm, ym, zm;
	PCType pc_type;

	if (efs_log(slv, EFS_LOG_STATUS)) {
		ierr = ef_io_print(slv->comm, "Setting up electric field solver");CHKERRQ(ierr);
	}

	slv->ts = 0;

	if (efs_log(slv, EFS_LOG_RESIDUAL)) {
		ierr = PetscOptionsSetValue("-ksp_monitor_short", NULL);CHKERRQ(ierr);
	}

	ierr = DMDASetFieldName(slv->dm, 0,"potential");CHKERRQ(ierr);

	if (slv->grid.nd == 2) {
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr);
		slv->dmap = ef_dmap_create_2d(xs - offset[0], ys - offset[1], xm, ym, stride);
	} else if (slv->grid.nd == 3) {
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);
		slv->dmap = ef_dmap_create_3d(xs - offset[0], ys - offset[1], zs - offset[2], xm, ym, zm, stride);
	} else {
		SETERRQ1(PETSC_COMM_WORLD, PETSC_ERR_SUP, "Unsupported dimmension: %d", slv->grid.nd);
	}
	ierr = ef_callback_create(&slv->callback);CHKERRQ(ierr);

	ierr = KSPCreate(slv->comm, &slv->ksp);CHKERRQ(ierr);
	if (efs_log(slv, EFS_LOG_EIGS)) {
		ierr = KSPSetComputeEigenvalues(slv->ksp, PETSC_TRUE);CHKERRQ(ierr);
	}
	ierr = KSPSetDM(slv->ksp, slv->dm);CHKERRQ(ierr);
	ierr = KSPGetPC(slv->ksp, &slv->pc);CHKERRQ(ierr);
	ierr = PCSetType(slv->pc, PCMG);CHKERRQ(ierr);
	if (slv->options.galerkin) {
		ierr = PCMGSetGalerkin(slv->pc, PETSC_TRUE);CHKERRQ(ierr);
	} else {
		ierr = PCMGSetGalerkin(slv->pc, PETSC_FALSE);CHKERRQ(ierr);
	}

	ierr = KSPSetComputeOperators(slv->ksp, slv->callback->matrix, slv);CHKERRQ(ierr);
	ierr = KSPSetComputeRHS(slv->ksp, slv->callback->rhs, slv);CHKERRQ(ierr);
	ierr = KSPSetComputeInitialGuess(slv->ksp, slv->callback->guess, slv);CHKERRQ(ierr);
	ierr = KSPSetFromOptions(slv->ksp);CHKERRQ(ierr);

	ierr = PCGetType(slv->pc, &pc_type);CHKERRQ(ierr);

	ierr = PCMGGetLevels(slv->pc, &slv->options.levels);CHKERRQ(ierr);
	if (slv->options.levels < 1) slv->options.levels++;
	ierr = PCMGGetGalerkin(slv->pc, &slv->options.galerkin);CHKERRQ(ierr);
	if (strcmp(pc_type, PCGAMG) == 0
	    || strcmp(pc_type, PCHYPRE) == 0) slv->options.galerkin = 1;
	if (!slv->options.galerkin) {
		slv->levels = (ef_level*) malloc(slv->options.levels*sizeof(ef_level));
		// setup callback for transforming rhs on coarse levels
	} else {
		slv->levels = (ef_level*) malloc(sizeof(ef_level));
	}
	ierr = ef_fd_create(&slv->fd, EF_FD_STANDARD_O2);CHKERRQ(ierr);
	ierr = ef_operator_create(&slv->op, slv->levels, slv->fd, slv->grid.nd);CHKERRQ(ierr);
	ierr = ef_boundary_create(&slv->boundary, slv->levels, slv->options.levels,
	                          slv->dmap, &slv->state, slv->fd);CHKERRQ(ierr);
	slv->op->axisymmetric = slv->options.axisymmetric;
	slv->boundary->axisymmetric = slv->options.axisymmetric;
	ierr = DMSetMatType(slv->dm, MATAIJ);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].eps);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].g);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].ag);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].gcomp);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].scale);CHKERRQ(ierr);
	ierr = DMCreateGlobalVector(slv->dm, &slv->levels[0].nscale);CHKERRQ(ierr);
	ierr = VecSet(slv->levels[0].g, 0);CHKERRQ(ierr);
	ierr = VecSet(slv->levels[0].gcomp, 1);CHKERRQ(ierr);
	ierr = VecSet(slv->levels[0].scale, 1);CHKERRQ(ierr);
	ierr = VecSet(slv->levels[0].nscale, 1);CHKERRQ(ierr);

	ierr = DMCoarsenHookAdd(slv->dm, slv->callback->coarsen, slv->callback->restrct, slv);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode efs_solve(efs *slv)
{
	PetscErrorCode ierr;
	PetscInt i, j, k, xs, ys, zs, xm, ym, zm;
	PetscInt ngx,ngy,ngz;
	DMBoundaryType bx, by, bz;
	slv->ts++;

	ierr = KSPSolve(slv->ksp, NULL, NULL);CHKERRQ(ierr);
	if (efs_log(slv, EFS_LOG_STATUS)) {
		ierr = ef_io_print(slv->comm, "Solve completed");CHKERRQ(ierr);
	}
	ierr = KSPGetSolution(slv->ksp, &slv->x);CHKERRQ(ierr);
	ierr = DMDAGetInfo(slv->dm, 0, &ngx, &ngy, &ngz,0,0,0,0,0, &bx, &by, &bz,0);CHKERRQ(ierr);

	if (efs_log(slv, EFS_LOG_PROBLEM)) {
		PetscViewer xout;
		ierr = PetscViewerASCIIOpen(slv->comm, "x.txt", &xout);CHKERRQ(ierr);
		ierr = VecView(slv->x, xout);CHKERRQ(ierr);
	}


	ierr = DMDAGetCorners(slv->dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);

	if (slv->grid.nd == 2) {
		Vec loc_x;
		PetscScalar **xvec;
		double **phi;

		ierr = ef_dmap_get(slv->dmap, slv->state.phi, &phi);CHKERRQ(ierr);
		ierr = DMGetLocalVector(slv->dm, &loc_x);CHKERRQ(ierr);
		ierr = DMGlobalToLocalBegin(slv->dm, slv->x, INSERT_VALUES, loc_x);CHKERRQ(ierr);
		ierr = DMGlobalToLocalEnd(slv->dm, slv->x, INSERT_VALUES, loc_x);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, loc_x, &xvec);CHKERRQ(ierr);
		if ((by == DM_BOUNDARY_PERIODIC) && (ys + ym == ngy)) {
			for (i = xs; i < xs + xm; i++) {
				xvec[ngy-1][i] = xvec[ngy][i];
			}
		}

		if ((bx == DM_BOUNDARY_PERIODIC) && (xs + xm == ngx)) {
			for (j = ys; j < ys + ym; j++) {
				xvec[j][ngx-1] = xvec[j][ngx];
			}
		}

		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				phi[j][i] = xvec[j][i];
			}
		}

		ierr = ef_dmap_restore(slv->dmap, &phi);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, loc_x, &xvec);CHKERRQ(ierr);
		if (efs_log(slv, EFS_LOG_VTK)) {
			ierr = DMLocalToGlobalBegin(slv->dm, loc_x, INSERT_VALUES, slv->x);CHKERRQ(ierr);
			ierr = DMLocalToGlobalEnd(slv->dm, loc_x, INSERT_VALUES, slv->x);CHKERRQ(ierr);
			ierr = ef_io_vtkwrite(slv->dm, slv->x, "phi", slv->grid.id, slv->ts);CHKERRQ(ierr);
		}
	} else if (slv->grid.nd == 3) {
		PetscScalar ***xvec;
		double ***phi;
		Vec loc_x;

		ierr = ef_dmap_get(slv->dmap, slv->state.phi, &phi);CHKERRQ(ierr);
		ierr = DMGetLocalVector(slv->dm, &loc_x);CHKERRQ(ierr);
		ierr = DMGlobalToLocalBegin(slv->dm, slv->x, INSERT_VALUES, loc_x);CHKERRQ(ierr);
		ierr = DMGlobalToLocalEnd(slv->dm, slv->x, INSERT_VALUES, loc_x);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, loc_x, &xvec);CHKERRQ(ierr);
		if ((bz == DM_BOUNDARY_PERIODIC) && (zs + zm == ngz)) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					xvec[ngz-1][j][i] = xvec[ngz][j][i];
				}
			}
		}
		if ((by == DM_BOUNDARY_PERIODIC) && (ys + ym == ngy)) {
			for (k = zs; k < zs + zm; k++) {
				for (i = xs; i < xs + xm; i++) {
					xvec[k][ngy-1][i] = xvec[k][ngy][i];
				}
			}
		}
		if ((bx == DM_BOUNDARY_PERIODIC) && (xs + xm == ngx)) {
			for (k = zs; k < zs + zm; k++) {
				for (j = ys; j < ys + ym; j++) {
					xvec[k][j][ngx-1] = xvec[k][j][ngx];
				}
			}
		}
		for (k = zs; k < zs + zm; k++) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					phi[k][j][i] = xvec[k][j][i];
				}
			}
		}

		ierr = ef_dmap_restore(slv->dmap, &phi);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, loc_x, &xvec);CHKERRQ(ierr);
		if (efs_log(slv, EFS_LOG_VTK)) {
			ierr = DMLocalToGlobalBegin(slv->dm, loc_x, INSERT_VALUES, slv->x);CHKERRQ(ierr);
			ierr = DMLocalToGlobalEnd(slv->dm, loc_x, INSERT_VALUES, slv->x);CHKERRQ(ierr);
			ierr = ef_io_vtkwrite(slv->dm, slv->x, "phi", slv->grid.id, slv->ts);CHKERRQ(ierr);
		}
	}

	if (efs_log(slv, EFS_LOG_VTK)) {
		if (slv->state.sol) {
			ierr = ef_io_vtkwrite(slv->dm, slv->sol, "sol", slv->grid.id, 0);CHKERRQ(ierr);
			Vec err;
			ierr = VecDuplicate(slv->sol, &err);CHKERRQ(ierr);
			ierr = VecWAXPY(err, -1, slv->x, slv->sol);CHKERRQ(ierr);
			ierr = ef_io_vtkwrite(slv->dm, err, "err", slv->grid.id, 0);CHKERRQ(ierr);
			ierr = VecDestroy(&err);CHKERRQ(ierr);
		}
	}


	if (efs_log(slv, EFS_LOG_EIGS)) {
		PetscInt n, neig;
		PetscReal *r, *c;
		n = ngx*ngy;
		r = (PetscReal*) malloc(n*sizeof(PetscReal));
		c = (PetscReal*) malloc(n*sizeof(PetscReal));
		ierr = KSPComputeEigenvalues(slv->ksp, n, r, c, &neig);CHKERRQ(ierr);
		ierr = ef_io_eigwrite(r, c, neig);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode efs_set_state(efs *slv, double rhs[], double phi[], double eps[],
                             double debye[])
{
	PetscErrorCode ierr;

	slv->state.phi = phi;
	slv->state.eps = eps;
	slv->state.debye = debye;
	slv->state.rhs = rhs;
	slv->state.sol = NULL;

	if (slv->grid.nd == 2) {
		PetscScalar **epsvec;
		double **eps_state;
		PetscInt i, j;
		PetscInt xs, ys, xm, ym;
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->state.eps, &eps_state);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, slv->levels[0].eps, &epsvec);CHKERRQ(ierr);

		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				epsvec[j][i] = eps_state[j][i];
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &eps_state);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, slv->levels[0].eps, &epsvec);CHKERRQ(ierr);
	} else if (slv->grid.nd == 3) {
		PetscScalar ***epsvec;
		double ***eps_state;
		PetscInt i, j, k;
		PetscInt xs, ys, zs, xm, ym, zm;
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->state.eps, &eps_state);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, slv->levels[0].eps, &epsvec);CHKERRQ(ierr);
		for (k = zs; k < zs + zm; k++) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					epsvec[k][j][i] = eps_state[k][j][i];
				}
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &eps_state);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, slv->levels[0].eps, &epsvec);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode efs_set_sol(efs *slv, double sol[])
{
	PetscErrorCode ierr;

	ierr = DMCreateGlobalVector(slv->dm, &slv->sol);
	slv->state.sol = sol;

	if (slv->grid.nd == 2) {
		PetscScalar **svec;
		double **sol_state;
		PetscInt i, j;
		PetscInt xs, ys, xm, ym;
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->state.sol, &sol_state);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, slv->sol, &svec);CHKERRQ(ierr);

		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				svec[j][i] = sol_state[j][i];
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &sol_state);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, slv->sol, &svec);CHKERRQ(ierr);
	} else if (slv->grid.nd == 3) {
		PetscScalar ***svec;
		double ***sol_state;
		PetscInt i, j, k;
		PetscInt xs, ys, zs, xm, ym, zm;
		ierr = DMDAGetCorners(slv->dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->state.sol, &sol_state);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, slv->sol, &svec);CHKERRQ(ierr);
		for (k = zs; k < zs + zm; k++) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					svec[k][j][i] = sol_state[k][j][i];
				}
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &sol_state);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, slv->sol, &svec);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode efs_set_grid(efs *slv, int is[], int ie[], int num_cells, double xyz[])
{
	PetscErrorCode ierr;
	PetscInt i, j, k;
	PetscInt xs, ys, zs, xm, ym, zm;
	DM cda;
	Vec gc;

	for (i = 0; i < slv->grid.nd; i++) {
		slv->grid.is[i] = is[i];
		slv->grid.ie[i] = ie[i];
	}
	slv->grid.num_cells = num_cells;
	slv->grid.xyz = xyz;

	ierr = DMGetCoordinateDM(slv->dm, &cda);CHKERRQ(ierr);
	ierr = DMGetCoordinates(slv->dm, &gc);CHKERRQ(ierr);
	ierr = DMDAGetCorners(cda, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);
	if (slv->grid.nd == 2) {
		DMDACoor2d **coors;
		double **x, **y;
		ierr = ef_dmap_get(slv->dmap, slv->grid.xyz, &x);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->grid.xyz + num_cells, &y);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(cda, gc, &coors);CHKERRQ(ierr);

		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				coors[j][i].x = x[j][i];
				coors[j][i].y = y[j][i];
			}
		}

		ierr = ef_dmap_restore(slv->dmap, &x);CHKERRQ(ierr);
		ierr = ef_dmap_restore(slv->dmap, &y);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(cda, gc, &coors);CHKERRQ(ierr);
	} else if (slv->grid.nd == 3) {
		DMDACoor3d ***coors;
		double ***x, ***y, ***z;
		ierr = DMDAVecGetArray(cda, gc, &coors);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->grid.xyz, &x);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->grid.xyz + num_cells, &y);CHKERRQ(ierr);
		ierr = ef_dmap_get(slv->dmap, slv->grid.xyz + 2*num_cells, &z);CHKERRQ(ierr);

		for (k = zs; k < zs + zm; k++) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					coors[k][j][i].x = x[k][j][i];
					coors[k][j][i].y = y[k][j][i];
					coors[k][j][i].z = z[k][j][i];
				}
			}
		}

		ierr = ef_dmap_restore(slv->dmap, &x);CHKERRQ(ierr);
		ierr = ef_dmap_restore(slv->dmap, &y);CHKERRQ(ierr);
		ierr = ef_dmap_restore(slv->dmap, &z);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(cda, gc, &coors);CHKERRQ(ierr);
	}

	ierr = ef_metric_create(&slv->levels[0].metric, slv->dm, slv->fd);CHKERRQ(ierr);

	return 0;
}


int efs_log(efs *slv, efs_log_level level)
{
	return slv->options.log_level & level;
}


void efs_set_log(efs *slv, efs_log_level level)
{
	slv->options.log_level = level;
}


PetscErrorCode efs_destroy(efs *slv)
{
	PetscErrorCode ierr;
	int i;

	ierr = ef_boundary_destroy(slv->boundary);CHKERRQ(ierr);
	ierr = ef_operator_destroy(slv->op);CHKERRQ(ierr); free(slv->op);
	ierr = ef_fd_destroy(slv->fd);CHKERRQ(ierr); free(slv->fd);
	ef_dmap_destroy(slv->dmap); free(slv->dmap);
	if (slv->options.galerkin) {
		ierr = ef_metric_destroy(slv->levels[0].metric);CHKERRQ(ierr);
		free(slv->levels[0].metric);
	} else {
		for (i = 0; i < slv->options.levels; i++) {
			ierr = ef_metric_destroy(slv->levels[i].metric);CHKERRQ(ierr);
			free(slv->levels[i].metric);
		}
	}
	free(slv->levels);
	free(slv->callback);

	ierr = DMDestroy(&slv->dm);CHKERRQ(ierr);
	ierr = KSPDestroy(&slv->ksp);CHKERRQ(ierr);

	return 0;
}

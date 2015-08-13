#include <stdlib.h>

#include "ef_solver.h"
#include "ef_callback.h"
#include "ef_io.h"


static PetscErrorCode restrict_hook(DM dmf, Mat rmat, Vec rscale, Mat inject, DM dmc, void *user_ctx)
{
	PetscErrorCode ierr;
	efs *slv = (efs*) user_ctx;
	PetscInt clevel;

	if (!slv->options.galerkin) {
		ierr = DMGetCoarsenLevel(dmc, &clevel);CHKERRQ(ierr);
		ierr = MatRestrict(rmat, slv->levels[clevel - 1].eps, slv->levels[clevel].eps);CHKERRQ(ierr);
		ierr = VecPointwiseMult(slv->levels[clevel].eps, slv->levels[clevel].eps, rscale);CHKERRQ(ierr);
		ierr = MatRestrict(rmat, slv->levels[clevel - 1].g, slv->levels[clevel].g);CHKERRQ(ierr);
		ierr = VecPointwiseMult(slv->levels[clevel].g, slv->levels[clevel].g, rscale);CHKERRQ(ierr);
	}

	return (0);
}


static PetscErrorCode coarsen_hook(DM dmf, DM dmc, void *user_ctx)
{
	PetscErrorCode ierr;
	efs *slv = (efs*) user_ctx;
	PetscInt clevel;

	if (!slv->options.galerkin) {
		ierr = DMGetCoarsenLevel(dmc, &clevel);CHKERRQ(ierr);
		ierr = DMCreateGlobalVector(dmc, &slv->levels[clevel].eps);CHKERRQ(ierr);
		ierr = DMCoarsenHookAdd(dmc, coarsen_hook, restrict_hook, slv);CHKERRQ(ierr);
		ierr = DMSetMatType(dmc, MATAIJ);CHKERRQ(ierr);
		ierr = ef_metric_create(&slv->levels[clevel].metric, dmc, slv->fd);CHKERRQ(ierr);
		ierr = ef_boundary_coarsen(slv->boundary, &slv->grid, dmc);CHKERRQ(ierr);
		ierr = VecDuplicate(slv->levels[clevel].eps, &slv->levels[clevel].g);CHKERRQ(ierr);
		ierr = VecDuplicate(slv->levels[clevel].eps, &slv->levels[clevel].ag);CHKERRQ(ierr);
		ierr = VecDuplicate(slv->levels[clevel].eps, &slv->levels[clevel].gcomp);CHKERRQ(ierr);
		ierr = VecDuplicate(slv->levels[clevel].eps, &slv->levels[clevel].scale);CHKERRQ(ierr);
		ierr = VecDuplicate(slv->levels[clevel].eps, &slv->levels[clevel].nscale);CHKERRQ(ierr);
		ierr = VecSet(slv->levels[clevel].ag, 0.0);CHKERRQ(ierr);
		ierr = VecSet(slv->levels[clevel].scale, 1.0);CHKERRQ(ierr);
		ierr = VecSet(slv->levels[clevel].nscale, 1.0);CHKERRQ(ierr);
		ierr = VecSet(slv->levels[clevel].gcomp, 1.0);
	}

	return (0);
}


static PetscErrorCode compute_guess(KSP ksp, Vec x, void *user_ctx)
{
	PetscErrorCode ierr;
	/* DM da; */

	/* ierr = KSPGetDM(ksp, &da);CHKERRQ(ierr); */

	/* PetscScalar **xvec; */
	/* PetscInt i, j, xs, ys, xm, ym; */
	/* ierr = DMDAGetCorners(da, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr); */
	/* ierr = DMDAVecGetArray(da, x, &xvec);CHKERRQ(ierr); */
	/* for (i = xs; i < xs + xm; i++) { */
	/* 	for (j = ys; j < ys + ym; j++) { */
	/* 		xvec[j][i] = rand(); */
	/* 	} */
	/* } */
	/* ierr = DMDAVecRestoreArray(da, x, &xvec);CHKERRQ(ierr); */
	ierr = VecSet(x, 1);CHKERRQ(ierr);

	return(0);
}


static PetscErrorCode compute_rhs(KSP ksp, Vec b, void *user_ctx)
{
	PetscErrorCode ierr;
	PetscInt i, j, k, xs, ys, zs, xm, ym, zm;

	efs *slv = (efs*) user_ctx;

	ierr = DMDAGetCorners(slv->dm, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);

	if (slv->grid.nd == 2) {
		double **rhs;
		PetscScalar **bvec;

		ierr = ef_dmap_get(slv->dmap, slv->state.rhs, &rhs);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, b, &bvec);CHKERRQ(ierr);
		if (slv->op->axisymmetric) {
			DM cda;
			DMDACoor2d **coors;
			Vec gc;

			ierr = DMGetCoordinateDM(slv->dm, &cda);CHKERRQ(ierr);
			ierr = DMGetCoordinates(slv->dm, &gc);CHKERRQ(ierr);
			ierr = DMDAVecGetArray(cda, gc, &coors);CHKERRQ(ierr);

			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					bvec[j][i] = rhs[j][i] * coors[j][i].x;
				}
			}

			ierr = DMDAVecRestoreArray(cda, gc, &coors);CHKERRQ(ierr);
		} else {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					bvec[j][i] = rhs[j][i];
				}
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &rhs);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, b, &bvec);CHKERRQ(ierr);
	} else {
		double ***rhs;
		double ***bvec;

		ierr = ef_dmap_get(slv->dmap, slv->state.rhs, &rhs);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(slv->dm, b, &bvec);CHKERRQ(ierr);
		for (k = zs; k < zs + zm; k++) {
			for (j = ys; j < ys + ym; j++) {
				for (i = xs; i < xs + xm; i++) {
					bvec[k][j][i] = rhs[k][j][i];
				}
			}
		}
		ierr = ef_dmap_restore(slv->dmap, &rhs);CHKERRQ(ierr);
		ierr = DMDAVecRestoreArray(slv->dm, b, &bvec);CHKERRQ(ierr);
	}

	ierr = ef_boundary_apply_rhs(slv->boundary, slv->dm);CHKERRQ(ierr);
	slv->rhs = b;

	if (efs_log(slv, EFS_LOG_PROBLEM)) {
		PetscViewer bout;
		PetscViewerASCIIOpen(slv->comm, "b.txt", &bout);
		ierr = VecView(b, bout);CHKERRQ(ierr);
	}

	return(0);
}


static PetscErrorCode compute_matrix(KSP ksp, Mat A, Mat B, void *user_ctx)
{
	PetscErrorCode ierr;
	PetscInt level;
	efs *slv = (efs*) user_ctx;
	DM da;

	ierr = KSPGetDM(ksp, &da);CHKERRQ(ierr);
	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ierr = ef_operator_assemble(slv->op, B, da);CHKERRQ(ierr);
	ierr = ef_boundary_apply(slv->boundary, B, da);CHKERRQ(ierr);

	ierr = MatAssemblyBegin(B, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
	ierr = MatAssemblyEnd(B, MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);

	if (efs_log(slv, EFS_LOG_STATUS)) {
		ierr = ef_io_print(slv->comm, "Matrix assembled");CHKERRQ(ierr);
	}

	if (efs_log(slv, EFS_LOG_PROBLEM)) {
		PetscViewer vout;
		ierr = PetscViewerBinaryOpen(slv->comm, "a.dat", FILE_MODE_WRITE, &vout);CHKERRQ(ierr);
		ierr = MatView(B, vout);CHKERRQ(ierr);
		ierr = PetscViewerDestroy(&vout);CHKERRQ(ierr);
	}

	if (level == 0) {
		ierr = slv->callback->presolve(slv->ksp, slv->rhs, slv->rhs, (void*) &slv->levels[0]);CHKERRQ(ierr);
	}

	return (0);
}


static PetscErrorCode coarse_presolve(KSP ksp, Vec b, Vec x, void *ctx)
{
	PetscErrorCode ierr;
	ef_level *level = (ef_level*) ctx;

	ierr = VecPointwiseMult(b, level->gcomp, b);CHKERRQ(ierr);
	ierr = VecPointwiseMult(b, level->scale, b);CHKERRQ(ierr);
	ierr = VecAXPY(b, -1, level->ag);CHKERRQ(ierr);
	// Boundary conditions are now negative (add twice to correct sign)
	ierr = VecAXPY(b, 2, level->g);CHKERRQ(ierr);
	//ierr = VecPointwiseMult(b, level->scale, b);CHKERRQ(ierr);
	ierr = VecPointwiseMult(b, level->nscale, b);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode ef_callback_create(ef_callback **efcall)
{
	ef_callback *efc;

	efc = (ef_callback*) malloc(sizeof(ef_callback));

	efc->restrct = &restrict_hook;
	efc->coarsen = &coarsen_hook;
	efc->guess = &compute_guess;
	efc->rhs = &compute_rhs;
	efc->matrix = &compute_matrix;
	efc->presolve = &coarse_presolve;

	*efcall = efc;
	return 0;
}

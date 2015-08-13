#include <stdlib.h>

#include "ef_operator.h"
#include "ef_formula.h"


static inline double have(double v1, double v2)
{
	return 2.0/((1.0/v1) + (1.0/v2));
}


static PetscErrorCode centered_2d(ef_operator *op, Mat A, DM da)
{
	PetscErrorCode ierr;
	DMBoundaryType bx, by;
	DM cda;
	DMDACoor2d **coors;
	Vec lc;
	PetscInt i,j,k,xs,ys,xm,ym,ngx,ngy;
	PetscInt ncol;
	PetscScalar v[9];
	PetscInt level;
	MatStencil row, col[9];
	Vec leps;
	double epsh[9];
	PetscScalar **eps, **scale;
	PetscScalar **jac[4], **jac2[6];
	PetscScalar **x_s, **x_t, **y_s, **y_t;
	PetscScalar **x_ss, **x_tt, **y_ss, **y_tt, **x_st, **y_st;
	double acc;

	ef_metric *met;

	enum dir {
		O = 0, N, S, E, W, NW, NE, SW, SE
	};

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	met = op->levels[level].metric;

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0, &bx, &by,0,0);CHKERRQ(ierr);

	ierr = DMDAGetCorners(da, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr);
	ierr = DMGetLocalVector(da, &leps);CHKERRQ(ierr);
	ierr = DMGlobalToLocalBegin(da, op->levels[level].eps, INSERT_VALUES, leps);CHKERRQ(ierr);
	ierr = DMGlobalToLocalEnd(da, op->levels[level].eps, INSERT_VALUES, leps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, leps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, op->levels[level].scale, &scale);CHKERRQ(ierr);

	if (op->axisymmetric) {
		ierr = DMGetCoordinateDM(da, &cda);CHKERRQ(ierr);
		ierr = DMGetCoordinatesLocal(da, &lc);CHKERRQ(ierr);
		ierr = DMDAVecGetArray(cda, lc, &coors);CHKERRQ(ierr);
	}

	for (i = 0; i < 4; i++) {
		ierr = DMDAVecGetArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
	}

	for (i = 0; i < 6; i++) {
		ierr = DMDAVecGetArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	x_s = jac[met->t2map[0][0]];
	x_t = jac[met->t2map[0][1]];
	y_s = jac[met->t2map[1][0]];
	y_t = jac[met->t2map[1][1]];

	x_ss = jac2[met->t2map[0][0]];
	x_tt = jac2[met->t2map[0][1]];
	y_ss = jac2[met->t2map[1][0]];
	y_tt = jac2[met->t2map[1][1]];
	x_st = jac2[met->t2map[0][2]];
	y_st = jac2[met->t2map[1][2]];

	double f_s[9];
	double f_t[9];
	double f_ss[9];
	double f_tt[9];
	double f_st[9];

	for (i = 0; i < 9; i++) {
		f_s[i] = 0;
		f_t[i] = 0;
		f_ss[i] = 0;
		f_tt[i] = 0;
		f_st[i] = 0;
	}

	f_s[W] = .5; f_s[E] = -.5;
	f_t[S] = .5; f_t[N] = -.5;
	f_ss[W] = -1; f_ss[O] = 2; f_ss[E] = -1;
	f_tt[S] = -1; f_tt[O] = 2; f_tt[N] = -1;
	f_st[SW] = .25; f_st[NW] = -.25; f_st[SE] = -.25; f_st[NE] = .25;

	for (j = ys; j < ys + ym; j++) {
		for (i = xs; i < xs + xm; i++) {
			if ((i != 0 || bx == DM_BOUNDARY_PERIODIC) &&
			    (j != 0 || by == DM_BOUNDARY_PERIODIC) &&
			    (i != ngx-1 || bx == DM_BOUNDARY_PERIODIC) &&
			    (j != ngy-1 || by == DM_BOUNDARY_PERIODIC)) {
				// TODO: major performance concerns
				// harmonic averaging
				epsh[N] = have(eps[j][i], eps[j+1][i]);
				epsh[S] = have(eps[j][i], eps[j-1][i]);
				epsh[W] = have(eps[j][i], eps[j][i-1]);
				epsh[E] = have(eps[j][i], eps[j][i+1]);
				epsh[SW] = have(have(eps[j][i], eps[j][i-1]),
				                have(eps[j-1][i], eps[j-1][i-1]));
				epsh[NW] = have(have(eps[j][i], eps[j][i-1]),
				                have(eps[j+1][i], eps[j+1][i-1]));
				epsh[SE] = have(have(eps[j][i], eps[j][i+1]),
				                have(eps[j-1][i], eps[j-1][i+1]));
				epsh[NE] = have(have(eps[j][i], eps[j][i+1]),
				                have(eps[j+1][i], eps[j+1][i+1]));

				if (op->axisymmetric) {
					epsh[E] = epsh[E] * (coors[j][i+1].x + coors[j][i].x) * .5;
					epsh[W] = epsh[W] * (coors[j][i].x + coors[j][i-1].x) * .5;
					epsh[N] = epsh[N] * 1.0 / coors[j][i].x;
					epsh[S] = epsh[S] * 1.0 / coors[j][i].x;
				}

				row.i = i; row.j = j;

				/* printf("%f %f  %f %f\n", x_s[j][i], x_t[j][i], y_s[j][i], y_t[j][i]); */
				/* printf("%f %f  %f %f\n", x_ss[j][i], x_tt[j][i], y_ss[j][i], y_tt[j][i]); */
				/* printf("%f %f\n", x_st[j][i], y_st[j][i]); */

				acc = 0;
				scale[j][i] = x_s[j][i] * y_t[j][i] - y_s[j][i] * x_t[j][i];
				for (k = 1; k < 9; k++) {
					v[k] = epsh[k]*FORMULA_2D*scale[j][i];
					acc += v[k];
				}
				v[O] = -1 * acc;

				col[SE].i = i+1; col[SE].j = j-1;
				col[S].i = i; col[S].j = j-1;
				col[SW].i = i-1; col[SW].j = j-1;
				col[W].i = i-1; col[W].j = j;
				col[O].i = i; col[O].j = j;
				col[E].i = i+1; col[E].j = j;
				col[NE].i = i+1; col[NE].j = j+1;
				col[N].i = i; col[N].j = j+1;
				col[NW].i = i-1; col[NW].j = j+1;

				ncol = 9;
				if (bx == DM_BOUNDARY_PERIODIC) {
					if (i == 0) {
						col[W].i--;
						col[SW].i--;
						col[NW].i--;
					} else if (i == ngx - 1) {
						v[0] = 1;
						ncol = 1;
					} else if (i == ngx-2) {
						col[E].i++;
						col[SE].i++;
						col[NE].i++;
					}
				} else if (by == DM_BOUNDARY_PERIODIC) {
					if (j == 0) {
						col[S].j--;
						col[SW].j--;
						col[SE].j--;
					} else if (j == ngy - 1) {
						v[0] = 1;
						ncol = 1;
					} else if (j == ngy - 2) {
						col[N].j++;
						col[NW].j++;
						col[NE].j++;
					}
				}

				ierr = MatSetValuesStencil(A, 1, &row, ncol, col, v, INSERT_VALUES);CHKERRQ(ierr);
			} else {
				scale[j][i] = x_s[j][i] * y_t[j][i] - y_s[j][i] * x_t[j][i];
			}
		}
	}

	ierr = DMDAVecRestoreArray(da, leps, &eps);CHKERRQ(ierr);
	ierr = DMRestoreLocalVector(da, &leps);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, op->levels[level].scale, &scale);CHKERRQ(ierr);
	for (i = 0; i < 4; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);

	}

	for (i = 0; i < 6; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	if (op->axisymmetric) {
		ierr = DMDAVecRestoreArray(cda, lc, &coors);CHKERRQ(ierr);
	}

	return 0;
}


static PetscErrorCode centered_3d(ef_operator *op, Mat A, DM da)
{
	PetscErrorCode ierr;
	DMBoundaryType bx, by, bz;
	PetscInt i,j,k,l,xs,ys,zs,xm,ym,zm,ngx,ngy,ngz;
	PetscScalar v[19];
	PetscInt level;
	PetscInt ncol;
	double acc;
	MatStencil row, col[19];
	double epsh[19];
	Vec leps;
	PetscScalar ***eps, ***scale;
	PetscScalar ***jac[9], ***jac2[18];
	PetscScalar ***x_r, ***x_s, ***x_t, ***y_r, ***y_s, ***y_t,
		        ***z_r, ***z_s, ***z_t;
	PetscScalar ***x_rr, ***x_ss, ***x_tt, ***y_rr, ***y_ss, ***y_tt,
		        ***z_rr, ***z_ss, ***z_tt, ***x_rs, ***x_rt, ***x_st,
		        ***y_rs, ***y_rt, ***y_st, ***z_rs, ***z_rt, ***z_st;

	ef_metric *met;

	enum dir {
		O = 0, N, S, E, W, F, B, NW, NE, SW, SE, NF, NB, SF, SB, FE, FW, BE, BW
	};

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	met = op->levels[level].metric;

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0,&bx,&by,&bz,0);CHKERRQ(ierr);

	ierr = DMDAGetCorners(da, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);

	ierr = DMGetLocalVector(da, &leps);CHKERRQ(ierr);
	ierr = DMGlobalToLocalBegin(da, op->levels[level].eps, INSERT_VALUES, leps);CHKERRQ(ierr);
	ierr = DMGlobalToLocalEnd(da, op->levels[level].eps, INSERT_VALUES, leps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, leps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, op->levels[level].scale, &scale);CHKERRQ(ierr);

	for (i = 0; i < 9; i++) {
		ierr = DMDAVecGetArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
	}

	for (i = 0; i < 18; i++) {
		ierr = DMDAVecGetArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	x_r = jac[met->t3map[0][0]];
	x_s = jac[met->t3map[0][1]];
	x_t = jac[met->t3map[0][2]];
	y_r = jac[met->t3map[1][0]];
	y_s = jac[met->t3map[1][1]];
	y_t = jac[met->t3map[1][2]];
	z_r = jac[met->t3map[2][0]];
	z_s = jac[met->t3map[2][1]];
	z_t = jac[met->t3map[2][2]];

	x_rr = jac2[met->t3map[0][0]];
	x_ss = jac2[met->t3map[0][1]];
	x_tt = jac2[met->t3map[0][2]];
	y_rr = jac2[met->t3map[1][0]];
	y_ss = jac2[met->t3map[1][1]];
	y_tt = jac2[met->t3map[1][2]];
	z_rr = jac2[met->t3map[2][0]];
	z_ss = jac2[met->t3map[2][1]];
	z_tt = jac2[met->t3map[2][2]];

	x_rs = jac2[met->t3map[0][3]];
	y_rs = jac2[met->t3map[1][3]];
	z_rs = jac2[met->t3map[2][3]];

	x_rt = jac2[met->t3map[0][4]];
	y_rt = jac2[met->t3map[1][4]];
	z_rt = jac2[met->t3map[2][4]];

	x_st = jac2[met->t3map[0][5]];
	y_st = jac2[met->t3map[1][5]];
	z_st = jac2[met->t3map[2][5]];


	double f_r[19];
	double f_s[19];
	double f_t[19];
	double f_rr[19];
	double f_ss[19];
	double f_tt[19];
	double f_rs[19];
	double f_rt[19];
	double f_st[19];

	for (i = 0; i < 19; i++) f_r[i] = f_s[i] = f_t[i] = f_rr[i] = f_ss[i] = f_tt[i] = f_rs[i] = f_rt[i] = f_st[i] = 0;

	f_r[E] = .5; f_r[W] = -.5;
	f_s[N] = .5; f_s[S] = -.5;
	f_t[F] = .5; f_t[B] = -.5;
	f_rr[O] = -2; f_rr[E] = 1; f_rr[W] = 1;
	f_ss[O] = -2; f_ss[N] = 1; f_ss[S] = 1;
	f_tt[O] = -2; f_tt[F] = 1; f_tt[B] = 1;
	f_rs[NE] = .25; f_rs[SE] = -.25; f_rs[NW] = -.25; f_rs[SW] = .25;
	f_rt[FE] = .25; f_rt[BE] = -.25; f_rt[FW] = -.25; f_rt[BW] = .25;
	f_st[NF] = .25; f_st[SF] = -.25; f_st[NB] = -.25; f_st[SB] = .25;

	for (k = zs; k < zs + zm; k++) {
		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				if ((i != 0 || bx == DM_BOUNDARY_PERIODIC) &&
				    (j != 0 || by == DM_BOUNDARY_PERIODIC) &&
				    (k != 0 || bz == DM_BOUNDARY_PERIODIC) &&
				    (i != ngx-1 || bx == DM_BOUNDARY_PERIODIC) &&
				    (j != ngy-1 || by == DM_BOUNDARY_PERIODIC) &&
				    (k != ngz-1 || bz == DM_BOUNDARY_PERIODIC)) {
					epsh[N] = have(eps[k][j][i], eps[k][j+1][i]);
					epsh[S] = have(eps[k][j][i], eps[k][j-1][i]);
					epsh[W] = have(eps[k][j][i], eps[k][j][i-1]);
					epsh[E] = have(eps[k][j][i], eps[k][j][i+1]);
					epsh[F] = have(eps[k][j][i], eps[k+1][j][i]);
					epsh[B] = have(eps[k][j][i], eps[k-1][j][i]);
					epsh[SW] = have(epsh[W],
					                have(eps[k][j-1][i], eps[k][j-1][i-1]));
					epsh[NW] = have(epsh[W],
					                have(eps[k][j+1][i], eps[k][j+1][i-1]));
					epsh[SE] = have(epsh[E],
					                have(eps[k][j-1][i], eps[k][j-1][i+1]));
					epsh[NE] = have(epsh[E],
					                have(eps[k][j+1][i], eps[k][j+1][i+1]));
					epsh[FE] = have(epsh[E],
					                have(eps[k+1][j][i], eps[k+1][j][i+1]));
					epsh[BE] = have(epsh[E],
					                have(eps[k-1][j][i], eps[k-1][j][i+1]));
					epsh[FW] = have(epsh[W],
					                have(eps[k+1][j][i], eps[k+1][j][i-1]));
					epsh[BW] = have(epsh[W],
					                have(eps[k-1][j][i], eps[k-1][j][i-1]));
					epsh[NF] = have(epsh[F],
					                have(eps[k][j+1][i], eps[k+1][j+1][i]));
					epsh[SF] = have(epsh[F],
					                have(eps[k][j-1][i], eps[k+1][j-1][i]));
					epsh[NB] = have(epsh[B],
					                have(eps[k][j+1][i], eps[k-1][j+1][i]));
					epsh[SB] = have(epsh[B],
					                have(eps[k][j-1][i], eps[k-1][j-1][i]));

					row.i = i; row.j = j; row.k = k;

		   /* 			printf("%f %f %f  %f %f %f  %f %f %f\n", x_r[k][j][i], x_s[k][j][i], x_t[k][j][i], */
	       /* y_r[k][j][i], y_s[k][j][i], y_t[k][j][i], */
	       /* z_r[k][j][i], z_s[k][j][i], z_t[k][j][i]); */
					/* printf("%f %f %f  %f %f %f  %f %f %f\n", */
					/*        x_rr[k][j][i], x_ss[k][j][i], x_tt[k][j][i], */
					/*        y_rr[k][j][i], y_ss[k][j][i], y_tt[k][j][i], */
					/*        z_rr[k][j][i], z_ss[k][j][i], z_tt[k][j][i]); */


		   			/* printf("%f %f %f  %f %f %f  %f %f %f\n", */
					/*        x_rs[k][j][i], x_rt[k][j][i], x_st[k][j][i], */
					/*        y_rs[k][j][i], y_rt[k][j][i], y_st[k][j][i], */
					/*        z_rs[k][j][i], z_rt[k][j][i], z_st[k][j][i]); */

					acc = 0;
					scale[k][j][i] = (x_r[k][j][i] * (y_s[k][j][i] * z_t[k][j][i]
					                                  - z_s[k][j][i] * y_t[k][j][i])
					                  - y_r[k][j][i] * (x_s[k][j][i] * z_t[k][j][i]
					                                    - z_s[k][j][i] * x_t[k][j][i])
					                  + z_r[k][j][i] * (x_s[k][j][i] * y_t[k][j][i]
					                                    - y_s[k][j][i] * x_t[k][j][i]));
					for (l = 1; l < 19; l++) {
						v[l] = -1 * epsh[l] * FORMULA_3D * scale[k][j][i];
						//if (isnan(v[l])) v[l] = 1;
						acc += v[l];
					}

					v[O] = -1 * acc;
					for (l = 0; l < 19; l++) {
						col[l].i = i;
						col[l].j = j;
						col[l].k = k;
					}
					col[N].j++;
					col[S].j--;
					col[E].i++;
					col[W].i--;
					col[F].k++;
					col[B].k--;
					col[NW].j++; col[NW].i--;
					col[NE].j++; col[NE].i++;
					col[SW].j--; col[SW].i--;
					col[SE].j--; col[SE].i++;
					col[NF].j++; col[NF].k++;
					col[NB].j++; col[NB].k--;
					col[SF].j--; col[SF].k++;
					col[SB].j--; col[SB].k--;
					col[FE].k++; col[FE].i++;
					col[FW].k++; col[FW].i--;
					col[BE].k--; col[BE].i++;
					col[BW].k--; col[BW].i--;

					ncol = 19;
					if (bx == DM_BOUNDARY_PERIODIC) {
						if (i == 0) {
							col[W].i--;
							col[SW].i--;
							col[NW].i--;
							col[FW].i--;
							col[BW].i--;
						} else if (i == ngx - 1) {
							v[0] = 1;
							ncol = 1;
						} else if (i == ngx-2) {
							col[E].i++;
							col[SE].i++;
							col[NE].i++;
							col[FE].i++;
							col[BE].i++;
						}
					} else if (by == DM_BOUNDARY_PERIODIC) {
						if (j == 0) {
							col[S].j--;
							col[SW].j--;
							col[SE].j--;
							col[SF].j--;
							col[SB].j--;
						} else if (j == ngy - 1) {
							v[0] = 1;
							ncol = 1;
						} else if (j == ngy - 2) {
							col[N].j++;
							col[NW].j++;
							col[NE].j++;
							col[NF].j++;
							col[NB].j++;
						}
					}
					else if (bz == DM_BOUNDARY_PERIODIC) {
						if (k == 0) {
							col[B].k--;
							col[NB].k--;
							col[SB].k--;
							col[BE].k--;
							col[BW].k--;
						} else if (k == ngz - 1) {
							v[0] = 1;
							ncol = 1;
						} else if (k == ngz - 2) {
							col[F].k++;
							col[NF].k++;
							col[SF].k++;
							col[FE].k++;
							col[FW].k++;
						}
					}

					ierr = MatSetValuesStencil(A, 1, &row, ncol, col, v, INSERT_VALUES);CHKERRQ(ierr);
				} else {
					scale[k][j][i] = (x_r[k][j][i] * (y_s[k][j][i] * z_t[k][j][i]
					                                  - z_s[k][j][i] * y_t[k][j][i])
					                  - y_r[k][j][i] * (x_s[k][j][i] * z_t[k][j][i]
					                                    - z_s[k][j][i] * x_t[k][j][i])
					                  + z_r[k][j][i] * (x_s[k][j][i] * y_t[k][j][i]
					                                    - y_s[k][j][i] * x_t[k][j][i]));
				}
			}
		}
	}

	ierr = DMDAVecRestoreArray(da, leps, &eps);CHKERRQ(ierr);
	for (i = 0; i < 9; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);

	}

	for (i = 0; i < 18; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_operator_create(ef_operator **efop, ef_level *levels, ef_fd *fd, int nd)
{
	ef_operator *op = (ef_operator*) malloc(sizeof(ef_operator));

	op->axisymmetric = 0;

	if (nd == 3)
		op->assemble = &centered_3d;
	else
		op->assemble = &centered_2d;
	op->levels = levels;
	op->fd = fd;

	*efop = op;
	return 0;
}


PetscErrorCode ef_operator_assemble(ef_operator *op, Mat A, DM da)
{
	PetscErrorCode ierr;

	ierr = op->assemble(op, A, da);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode ef_operator_destroy(ef_operator *efop)
{
	return 0;
}

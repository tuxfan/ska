#include <stdlib.h>
#include <math.h>

#include "ef_metric.h"


static PetscErrorCode compute_2d(ef_metric *met, DM da, ef_fd *fd)
{
	PetscErrorCode ierr;
	DM cda;
	DMBoundaryType bx, by;
	DMDACoor2d **coors;
	PetscInt i,j,xs,ys,xm,ym,ngx,ngy;
	PetscScalar **jac[4], **jac2[6];
	PetscScalar **x_s, **x_t, **y_s, **y_t;
	PetscScalar **x_ss, **x_tt, **y_ss, **y_tt, **x_st, **y_st;
	Vec cv;
	double *si, *sp, *sm;

	ierr = DMGetCoordinateDM(da, &cda);CHKERRQ(ierr);
	ierr = DMGetCoordinatesLocal(da, &cv);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(cda, cv, &coors);CHKERRQ(ierr);

	ierr = DMDAGetCorners(cda, &xs, &ys, 0, &xm, &ym, 0);CHKERRQ(ierr);
	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0, &bx, &by,0,0);CHKERRQ(ierr);

	//-------------- First order ------------------

	for (i = 0; i < 4; i++) {
		ierr = DMDAVecGetArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
	}

	x_s = jac[met->t2map[0][0]];
	x_t = jac[met->t2map[0][1]];
	y_s = jac[met->t2map[1][0]];
	y_t = jac[met->t2map[1][1]];

	si = fd->first[0]->v;
	sp = fd->first[1]->v;
	sm = fd->first[-1]->v;

	for (j = ys; j < ys + ym; j++) {
		for (i = xs; i < xs + xm; i++) {
			// TODO: move branching out of loop

			if ((i !=0 && i != ngx - 1) || bx == DM_BOUNDARY_PERIODIC) {
				if (i == 0) {
					x_s[j][i] = ((coors[j][i+1].x - coors[j][i].x) + (coors[j][i-1].x - coors[j][i-2].x)) * .5;
					y_s[j][i] = ((coors[j][i+1].y - coors[j][i].y) + (coors[j][i-1].y - coors[j][i-2].y)) * .5;
				} else if (i == ngx - 1) {
					x_s[j][i] = ((coors[j][i].x - coors[j][i-1].x) + (coors[j][i+2].x - coors[j][i+1].x)) * .5;
					y_s[j][i] = ((coors[j][i].y - coors[j][i-1].y) + (coors[j][i+2].y - coors[j][i+1].y)) * .5;
				} else {
					x_s[j][i] = si[1]*coors[j][i+1].x + si[-1]*coors[j][i-1].x;
					y_s[j][i] = si[1]*coors[j][i+1].y + si[-1]*coors[j][i-1].y;
				}
			} else if (i == 0) {
				x_s[j][i] = sm[0]*coors[j][i].x + sm[1]*coors[j][i+1].x + sm[2]*coors[j][i+2].x;
				y_s[j][i] = sm[0]*coors[j][i].y + sm[1]*coors[j][i+1].y + sm[2]*coors[j][i+2].y;
			} else { // i == ngx - 1
				x_s[j][i] = sp[0]*coors[j][i].x + sp[-1]*coors[j][i-1].x + sp[-2]*coors[j][i-2].x;
				y_s[j][i] = sp[0]*coors[j][i].y + sp[-1]*coors[j][i-1].y + sp[-2]*coors[j][i-2].y;
			}

			if ((j != 0 && j != ngy - 1) || by == DM_BOUNDARY_PERIODIC) {
				if (j == 0) {
					x_t[j][i] = ((coors[j+1][i].x - coors[j][i].x) + (coors[j-1][i].x - coors[j-2][i].x)) * .5;
					y_t[j][i] = ((coors[j+1][i].y - coors[j][i].y) + (coors[j-1][i].y - coors[j-2][i].y)) * .5;
				} else if (j == ngy - 1) {
					x_t[j][i] = ((coors[j][i].x - coors[j-1][i].x) + (coors[j+2][i].x - coors[j+1][i].x)) * .5;
					y_t[j][i] = ((coors[j][i].y - coors[j-1][i].y) + (coors[j+2][i].y - coors[j+1][i].y)) * .5;
				} else {
					x_t[j][i] = si[1]*coors[j+1][i].x + si[-1]*coors[j-1][i].x;
					y_t[j][i] = si[1]*coors[j+1][i].y + si[-1]*coors[j-1][i].y;
				}
			} else if (j == 0) {
				x_t[j][i] = sm[0]*coors[j][i].x + sm[1]*coors[j+1][i].x + sm[2]*coors[j+2][i].x;
				y_t[j][i] = sm[0]*coors[j][i].y + sm[1]*coors[j+1][i].y + sm[2]*coors[j+2][i].y;
			} else { // j == ngy - 1
				x_t[j][i] = sp[0]*coors[j][i].x + sp[-1]*coors[j-1][i].x + sp[-2]*coors[j-2][i].x;
				y_t[j][i] = sp[0]*coors[j][i].y + sp[-1]*coors[j-1][i].y + sp[-2]*coors[j-2][i].y;
			}
		}
	}

	for (i = 0; i < 4; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
	}

	//-------------- Second Order ------------------

	for (i = 0; i < 6; i++) {
		ierr = DMDAVecGetArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	x_ss = jac2[met->t2map[0][0]];
	x_tt = jac2[met->t2map[0][1]];
	y_ss = jac2[met->t2map[1][0]];
	y_tt = jac2[met->t2map[1][1]];
	x_st = jac2[met->t2map[0][2]];
	y_st = jac2[met->t2map[1][2]];

	si = fd->second[0]->v;
	sp = fd->second[1]->v;
	sm = fd->second[-1]->v;

	for (j = ys; j < ys + ym; j++) {
		for (i = xs; i < xs + xm; i++) {
			// TODO: move branches out of loop

			if ((i != 0 && i != ngx - 1) || bx == DM_BOUNDARY_PERIODIC) {
				if (i == 0) {
					x_ss[j][i] = ((coors[j][i+1].x - coors[j][i].x) - (coors[j][i-1].x - coors[j][i-2].x));
					y_ss[j][i] = ((coors[j][i+1].y - coors[j][i].y) - (coors[j][i-1].y - coors[j][i-2].y));
				} else if (i == ngx - 1) {
					x_ss[j][i] = ((coors[j][i+2].x - coors[j][i+1].x) - (coors[j][i].x - coors[j][i-1].x));
					y_ss[j][i] = ((coors[j][i+2].y - coors[j][i+1].y) - (coors[j][i].y - coors[j][i-1].y));
				} else {
					x_ss[j][i] = si[1]*coors[j][i+1].x + si[0]*coors[j][i].x + si[-1]*coors[j][i-1].x;
					y_ss[j][i] = si[1]*coors[j][i+1].y + si[0]*coors[j][i].y + si[-1]*coors[j][i-1].y;
				}
				x_st[j][i] = .25 * (coors[j+1][i+1].x - coors[j-1][i+1].x - coors[j+1][i-1].x + coors[j-1][i-1].x);
			} else if (i == 0) {
				x_ss[j][i] = sm[0]*coors[j][i].x + sm[1]*coors[j][i+1].x + sm[2]*coors[j][i+2].x + sm[3]*coors[j][i+3].x;
				y_ss[j][i] = sm[0]*coors[j][i].y + sm[1]*coors[j][i+1].y + sm[2]*coors[j][i+2].y + sm[3]*coors[j][i+3].y;
			} else { // i = ngx - 1
				x_ss[j][i] = sp[0]*coors[j][i].x + sp[-1]*coors[j][i-1].x + sp[-2]*coors[j][i-2].x + sp[-3]*coors[j][i-3].x;
				y_ss[j][i] = sp[0]*coors[j][i].y + sp[-1]*coors[j][i-1].y + sp[-2]*coors[j][i-2].y + sp[-3]*coors[j][i-3].y;
			}

			if ((j != 0 && j != ngy - 1) || by == DM_BOUNDARY_PERIODIC) {
				if (j == 0) {
					x_tt[j][i] = ((coors[j+1][i].x - coors[j][i].x) - (coors[j-1][i].x - coors[j-2][i].x));
					y_tt[j][i] = ((coors[j+1][i].y - coors[j][i].y) - (coors[j-1][i].y - coors[j-2][i].y));
				} else if (j == ngy - 1) {
					x_tt[j][i] = ((coors[j+2][i].x - coors[j+1][i].x) - (coors[j][i].x - coors[j-1][i].x));
					y_tt[j][i] = ((coors[j+2][i].y - coors[j+1][i].y) - (coors[j][i].y - coors[j-1][i].y));
				} else {
					x_tt[j][i] = si[1]*coors[j+1][i].x + si[0]*coors[j][i].x + si[-1]*coors[j-1][i].x;
					y_tt[j][i] = si[1]*coors[j+1][i].y + si[0]*coors[j][i].y + si[-1]*coors[j-1][i].y;
				}
				y_st[j][i] = .25 * (coors[j+1][i+1].y - coors[j-1][i+1].y - coors[j+1][i-1].y + coors[j-1][i-1].y);
			} else if (j == 0) {
				x_tt[j][i] = sm[0]*coors[j][i].x + sm[1]*coors[j+1][i].x + sm[2]*coors[j+2][i].x + sm[3]*coors[j+3][i].x;
				y_tt[j][i] = sm[0]*coors[j][i].y + sm[1]*coors[j+1][i].y + sm[2]*coors[j+2][i].y + sm[3]*coors[j+3][i].y;
			} else { // j = ngy - 1
				x_tt[j][i] = sp[0]*coors[j][i].x + sp[-1]*coors[j-1][i].x + sp[-2]*coors[j-2][i].x + sp[-3]*coors[j-3][i].x;
				y_tt[j][i] = sp[0]*coors[j][i].y + sp[-2]*coors[j-1][i].y + sp[-2]*coors[j-2][i].y + sp[-3]*coors[j-3][i].y;
			}
		}
	}

	for (i = 0; i < 6; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	ierr = DMDAVecRestoreArray(cda, cv, &coors);CHKERRQ(ierr);

	return 0;
}


static PetscErrorCode compute_3d(ef_metric *met, DM da, ef_fd *fd)
{
	PetscErrorCode ierr;
	DM cda;
	DMBoundaryType bx, by, bz;
	DMDACoor3d ***coors;
	PetscInt i,j,k,xs,ys,zs,xm,ym,zm,ngx,ngy,ngz;
	PetscScalar ***jac[9], ***jac2[18];
	PetscScalar ***x_r, ***x_s, ***x_t, ***y_r, ***y_s, ***y_t,
		        ***z_r, ***z_s, ***z_t;
	PetscScalar ***x_rr, ***x_ss, ***x_tt, ***y_rr, ***y_ss, ***y_tt,
		        ***z_rr, ***z_ss, ***z_tt, ***x_rs, ***x_rt, ***x_st,
		        ***y_rs, ***y_rt, ***y_st, ***z_rs, ***z_rt, ***z_st;

	Vec cv;
	double *si, *sp, *sm;

	ierr = DMGetCoordinateDM(da, &cda);CHKERRQ(ierr);
	ierr = DMGetCoordinatesLocal(da, &cv);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(cda, cv, &coors);CHKERRQ(ierr);

	ierr = DMDAGetCorners(cda, &xs, &ys, &zs, &xm, &ym, &zm);CHKERRQ(ierr);
	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0, &bx, &by, &bz,0);CHKERRQ(ierr);

	//-------------- First order ------------------

	for (i = 0; i < 9; i++) {
		ierr = DMDAVecGetArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
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

	si = fd->first[0]->v;
	sp = fd->first[1]->v;
	sm = fd->first[-1]->v;

	for (k = zs; k < zs + zm; k++) {
		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				// TODO: move branching out of loop (separate routines for interior and boundary derivatives)

				if ((i !=0 && i != ngx - 1) || bx == DM_BOUNDARY_PERIODIC) {
					if (i == 0) {
						x_r[k][j][i] = ((coors[k][j][i+1].x - coors[k][j][i].x) + (coors[k][j][i-1].x - coors[k][j][i-2].x)) * .5;
						y_r[k][j][i] = ((coors[k][j][i+1].y - coors[k][j][i].y) + (coors[k][j][i-1].y - coors[k][j][i-2].y)) * .5;
						z_r[k][j][i] = ((coors[k][j][i+1].z - coors[k][j][i].z) + (coors[k][j][i-1].z - coors[k][j][i-2].z)) * .5;
					} else if (i == ngx - 1) {
						x_r[k][j][i] = ((coors[k][j][i].x - coors[k][j][i-1].x) + (coors[k][j][i+2].x - coors[k][j][i+1].x)) * .5;
						y_r[k][j][i] = ((coors[k][j][i].y - coors[k][j][i-1].y) + (coors[k][j][i+2].y - coors[k][j][i+1].y)) * .5;
						z_r[k][j][i] = ((coors[k][j][i].z - coors[k][j][i-1].z) + (coors[k][j][i+2].z - coors[k][j][i+1].z)) * .5;
					} else {
						x_r[k][j][i] = si[1]*coors[k][j][i+1].x + si[-1]*coors[k][j][i-1].x;
						y_r[k][j][i] = si[1]*coors[k][j][i+1].y + si[-1]*coors[k][j][i-1].y;
						z_r[k][j][i] = si[1]*coors[k][j][i+1].z + si[-1]*coors[k][j][i-1].z;
					}
				} else if (i == 0) {
					x_r[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k][j][i+1].x + sm[2]*coors[k][j][i+2].x;
					y_r[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k][j][i+1].y + sm[2]*coors[k][j][i+2].y;
					z_r[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k][j][i+1].z + sm[2]*coors[k][j][i+2].z;
				} else { // i == ngx - 1
					x_r[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k][j][i-1].x + sp[-2]*coors[k][j][i-2].x;
					y_r[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k][j][i-1].y + sp[-2]*coors[k][j][i-2].y;
					z_r[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k][j][i-1].z + sp[-2]*coors[k][j][i-2].z;
				}


				if ((j != 0 && j != ngy - 1) || by == DM_BOUNDARY_PERIODIC) {
					if (j == 0) {
						x_s[k][j][i] = ((coors[k][j+1][i].x - coors[k][j][i].x) + (coors[k][j-1][i].x - coors[k][j-2][i].x)) * .5;
						y_s[k][j][i] = ((coors[k][j+1][i].y - coors[k][j][i].y) + (coors[k][j-1][i].y - coors[k][j-2][i].y)) * .5;
						z_s[k][j][i] = ((coors[k][j+1][i].z - coors[k][j][i].z) + (coors[k][j-1][i].z - coors[k][j-2][i].z)) * .5;
					} else if (j == ngy - 1) {
						x_s[k][j][i] = ((coors[k][j][i].x - coors[k][j-1][i].x) + (coors[k][j+2][i].x - coors[k][j+1][i].x)) * .5;
						y_s[k][j][i] = ((coors[k][j][i].y - coors[k][j-1][i].y) + (coors[k][j+2][i].y - coors[k][j+1][i].y)) * .5;
						z_s[k][j][i] = ((coors[k][j][i].z - coors[k][j-1][i].z) + (coors[k][j+2][i].z - coors[k][j+1][i].z)) * .5;
					} else {
						x_s[k][j][i] = si[1]*coors[k][j+1][i].x + si[-1]*coors[k][j-1][i].x;
						y_s[k][j][i] = si[1]*coors[k][j+1][i].y + si[-1]*coors[k][j-1][i].y;
						z_s[k][j][i] = si[1]*coors[k][j+1][i].z + si[-1]*coors[k][j-1][i].z;
					}
				} else if (j == 0) {
					x_s[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k][j+1][i].x + sm[2]*coors[k][j+2][i].x;
					y_s[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k][j+1][i].y + sm[2]*coors[k][j+2][i].y;
					z_s[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k][j+1][i].z + sm[2]*coors[k][j+2][i].z;
				} else { // j == ngy - 1
					x_s[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k][j-1][i].x + sp[-2]*coors[k][j-2][i].x;
					y_s[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k][j-1][i].y + sp[-2]*coors[k][j-2][i].y;
					z_s[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k][j-1][i].z + sp[-2]*coors[k][j-2][i].z;
				}

				if ((k != 0 && k != ngz - 1) || bz == DM_BOUNDARY_PERIODIC) {
					if (k == 0) {
						x_t[k][j][i] = ((coors[k+1][j][i].x - coors[k][j][i].x) + (coors[k-1][j][i].x - coors[k-2][j][i].x)) * .5;
						y_t[k][j][i] = ((coors[k+1][j][i].y - coors[k][j][i].y) + (coors[k-1][j][i].y - coors[k-2][j][i].y)) * .5;
						z_t[k][j][i] = ((coors[k+1][j][i].z - coors[k][j][i].z) + (coors[k-1][j][i].z - coors[k-2][j][i].z)) * .5;
					} else if (k == ngz - 1) {
						x_t[k][j][i] = ((coors[k][j][i].x - coors[k-1][j][i].x) + (coors[k+2][j][i].x - coors[k+1][j][i].x)) * .5;
						y_t[k][j][i] = ((coors[k][j][i].y - coors[k-1][j][i].y) + (coors[k+2][j][i].y - coors[k+1][j][i].y)) * .5;
						z_t[k][j][i] = ((coors[k][j][i].z - coors[k-1][j][i].z) + (coors[k+2][j][i].z - coors[k+1][j][i].z)) * .5;
					} else {
						x_t[k][j][i] = si[1]*coors[k+1][j][i].x + si[-1]*coors[k-1][j][i].x;
						y_t[k][j][i] = si[1]*coors[k+1][j][i].y + si[-1]*coors[k-1][j][i].y;
						z_t[k][j][i] = si[1]*coors[k+1][j][i].z + si[-1]*coors[k-1][j][i].z;
					}
				} else if (k == 0) {
					x_t[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k+1][j][i].x + sm[2]*coors[k+2][j][i].x;
					y_t[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k+1][j][i].y + sm[2]*coors[k+2][j][i].y;
					z_t[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k+1][j][i].z + sm[2]*coors[k+2][j][i].z;
				} else { // j == ngy - 1
					x_t[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k-1][j][i].x + sp[-2]*coors[k-2][j][i].x;
					y_t[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k-1][j][i].y + sp[-2]*coors[k-2][j][i].y;
					z_t[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k-1][j][i].z + sp[-2]*coors[k-2][j][i].z;
				}
			}
		}
	}

	for (i = 0; i < 9; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);
	}

	//-------------- Second Order ------------------

	for (i = 0; i < 18; i++) {
		ierr = DMDAVecGetArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

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

	si = fd->second[0]->v;
	sp = fd->second[1]->v;
	sm = fd->second[-1]->v;

	for (k = zs; k < zs + zm; k++) {
		for (j = ys; j < ys + ym; j++) {
			for (i = xs; i < xs + xm; i++) {
				// TODO: move branches out of loop

				if (i != 0 && j != 0 && k != 0 && i != ngx-1 && j != ngy-1 && k != ngz - 1) {
					x_rs[k][j][i] = .25 * (coors[k][j+1][i+1].x - coors[k][j-1][i+1].x - coors[k][j+1][i-1].x + coors[k][j-1][i-1].x);
					y_rs[k][j][i] = .25 * (coors[k][j+1][i+1].y - coors[k][j-1][i+1].y - coors[k][j+1][i-1].y + coors[k][j-1][i-1].y);
					z_rs[k][j][i] = .25 * (coors[k][j+1][i+1].z - coors[k][j-1][i+1].z - coors[k][j+1][i-1].z + coors[k][j-1][i-1].z);

					x_rt[k][j][i] = .25 * (coors[k+1][j][i+1].x - coors[k-1][j][i+1].x - coors[k+1][j][i-1].x + coors[k-1][j][i-1].x);
					y_rt[k][j][i] = .25 * (coors[k+1][j][i+1].y - coors[k-1][j][i+1].y - coors[k+1][j][i-1].y + coors[k-1][j][i-1].y);
					z_rt[k][j][i] = .25 * (coors[k+1][j][i+1].z - coors[k-1][j][i+1].z - coors[k+1][j][i-1].z + coors[k-1][j][i-1].z);

					x_st[k][j][i] = .25 * (coors[k+1][j+1][i].x - coors[k-1][j+1][i].x - coors[k+1][j-1][i].x + coors[k-1][j-1][i].x);
					y_st[k][j][i] = .25 * (coors[k+1][j+1][i].y - coors[k-1][j+1][i].y - coors[k+1][j-1][i].y + coors[k-1][j-1][i].y);
					z_st[k][j][i] = .25 * (coors[k+1][j+1][i].z - coors[k-1][j+1][i].z - coors[k+1][j-1][i].z + coors[k-1][j-1][i].z);
				}

				if ((i != 0 && i != ngx - 1) || bx == DM_BOUNDARY_PERIODIC) {
					if (i == 0) {
						x_rr[k][j][i] = ((coors[k][j][i+1].x - coors[k][j][i].x) - (coors[k][j][i-1].x - coors[k][j][i-2].x));
						y_rr[k][j][i] = ((coors[k][j][i+1].y - coors[k][j][i].y) - (coors[k][j][i-1].y - coors[k][j][i-2].y));
						z_rr[k][j][i] = ((coors[k][j][i+1].z - coors[k][j][i].z) - (coors[k][j][i-1].z - coors[k][j][i-2].z));
					} else if (i == ngx - 1) {
						x_rr[k][j][i] = ((coors[k][j][i+2].x - coors[k][j][i+1].x) - (coors[k][j][i].x - coors[k][j][i-1].x));
						y_rr[k][j][i] = ((coors[k][j][i+2].y - coors[k][j][i+1].y) - (coors[k][j][i].y - coors[k][j][i-1].y));
						z_rr[k][j][i] = ((coors[k][j][i+2].z - coors[k][j][i+1].z) - (coors[k][j][i].z - coors[k][j][i-1].z));
					} else {
						x_rr[k][j][i] = si[1]*coors[k][j][i+1].x + si[0]*coors[k][j][i].x + si[-1]*coors[k][j][i-1].x;
						y_rr[k][j][i] = si[1]*coors[k][j][i+1].y + si[0]*coors[k][j][i].y + si[-1]*coors[k][j][i-1].y;
						z_rr[k][j][i] = si[1]*coors[k][j][i+1].z + si[0]*coors[k][j][i].z + si[-1]*coors[k][j][i-1].z;
					}
				} else if (i == 0) {
					x_rr[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k][j][i+1].x + sm[2]*coors[k][j][i+2].x + sm[3]*coors[k][j][i+3].x;
					y_rr[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k][j][i+1].y + sm[2]*coors[k][j][i+2].y + sm[3]*coors[k][j][i+3].y;
					z_rr[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k][j][i+1].z + sm[2]*coors[k][j][i+2].z + sm[3]*coors[k][j][i+3].z;
				} else { // i = ngx - 1
					x_rr[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k][j][i-1].x + sp[-2]*coors[k][j][i-2].x + sp[-3]*coors[k][j][i-3].x;
					y_rr[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k][j][i-1].y + sp[-2]*coors[k][j][i-2].y + sp[-3]*coors[k][j][i-3].y;
					z_rr[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k][j][i-1].z + sp[-2]*coors[k][j][i-2].z + sp[-3]*coors[k][j][i-3].z;
				}

				if ((j != 0 && j != ngy - 1) || by == DM_BOUNDARY_PERIODIC) {
					if (j == 0) {
						x_ss[k][j][i] = ((coors[k][j+1][i].x - coors[k][j][i].x) - (coors[k][j-1][i].x - coors[k][j-2][i].x));
						y_ss[k][j][i] = ((coors[k][j+1][i].y - coors[k][j][i].y) - (coors[k][j-1][i].y - coors[k][j-2][i].y));
						z_ss[k][j][i] = ((coors[k][j+1][i].z - coors[k][j][i].z) - (coors[k][j-1][i].z - coors[k][j-2][i].z));
					} else if (j == ngy - 1) {
						x_ss[k][j][i] = ((coors[k][j+2][i].x - coors[k][j+1][i].x) - (coors[k][j][i].x - coors[k][j-1][i].x));
						y_ss[k][j][i] = ((coors[k][j+2][i].y - coors[k][j+1][i].y) - (coors[k][j][i].y - coors[k][j-1][i].y));
						z_ss[k][j][i] = ((coors[k][j+2][i].z - coors[k][j+1][i].z) - (coors[k][j][i].z - coors[k][j-1][i].z));
					} else {
						x_ss[k][j][i] = si[1]*coors[k][j+1][i].x + si[0]*coors[k][j][i].x + si[-1]*coors[k][j-1][i].x;
						y_ss[k][j][i] = si[1]*coors[k][j+1][i].y + si[0]*coors[k][j][i].y + si[-1]*coors[k][j-1][i].y;
						z_ss[k][j][i] = si[1]*coors[k][j+1][i].z + si[0]*coors[k][j][i].z + si[-1]*coors[k][j-1][i].z;
					}
				} else if (j == 0) {
					x_ss[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k][j+1][i].x + sm[2]*coors[k][j+2][i].x + sm[3]*coors[k][j+3][i].x;
					y_ss[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k][j+1][i].y + sm[2]*coors[k][j+2][i].y + sm[3]*coors[k][j+3][i].y;
					z_ss[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k][j+1][i].z + sm[2]*coors[k][j+2][i].z + sm[3]*coors[k][j+3][i].z;
				} else { // j = ngy - 1
					x_ss[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k][j-1][i].x + sp[-2]*coors[k][j-2][i].x + sp[-3]*coors[k][j-3][i].x;
					y_ss[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k][j-1][i].y + sp[-2]*coors[k][j-2][i].y + sp[-3]*coors[k][j-3][i].y;
					z_ss[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k][j-1][i].z + sp[-2]*coors[k][j-2][i].z + sp[-3]*coors[k][j-3][i].z;
				}

				if ((k != 0 && k != ngz - 1) || bz == DM_BOUNDARY_PERIODIC) {
					if (k == 0) {
						x_tt[k][j][i] = ((coors[k+1][j][i].x - coors[k][j][i].x) - (coors[k-1][j][i].x - coors[k-2][j][i].x));
						y_tt[k][j][i] = ((coors[k+1][j][i].y - coors[k][j][i].y) - (coors[k-1][j][i].y - coors[k-2][j][i].y));
						z_tt[k][j][i] = ((coors[k+1][j][i].z - coors[k][j][i].z) - (coors[k-1][j][i].z - coors[k-2][j][i].z));
					} else if (k == ngz - 1) {
						x_tt[k][j][i] = ((coors[k+2][j][i].x - coors[k+1][j][i].x) - (coors[k][j][i].x - coors[k-1][j][i].x));
						y_tt[k][j][i] = ((coors[k+2][j][i].y - coors[k+1][j][i].y) - (coors[k][j][i].y - coors[k-1][j][i].y));
						z_tt[k][j][i] = ((coors[k+2][j][i].z - coors[k+1][j][i].z) - (coors[k][j][i].z - coors[k-1][j][i].z));
					} else {
						x_tt[k][j][i] = si[1]*coors[k+1][j][i].x + si[0]*coors[k][j][i].x + si[-1]*coors[k-1][j][i].x;
						y_tt[k][j][i] = si[1]*coors[k+1][j][i].y + si[0]*coors[k][j][i].y + si[-1]*coors[k-1][j][i].y;
						z_tt[k][j][i] = si[1]*coors[k+1][j][i].z + si[0]*coors[k][j][i].z + si[-1]*coors[k-1][j][i].z;
					}
				} else if (k == 0) {
					x_tt[k][j][i] = sm[0]*coors[k][j][i].x + sm[1]*coors[k+1][j][i].x + sm[2]*coors[k+2][j][i].x + sm[3]*coors[k+3][j][i].x;
					y_tt[k][j][i] = sm[0]*coors[k][j][i].y + sm[1]*coors[k+1][j][i].y + sm[2]*coors[k+2][j][i].y + sm[3]*coors[k+3][j][i].y;
					z_tt[k][j][i] = sm[0]*coors[k][j][i].z + sm[1]*coors[k+1][j][i].z + sm[2]*coors[k+2][j][i].z + sm[3]*coors[k+3][j][i].z;
				} else { // j = ngy - 1
					x_tt[k][j][i] = sp[0]*coors[k][j][i].x + sp[-1]*coors[k-1][j][i].x + sp[-2]*coors[k-2][j][i].x + sp[-3]*coors[k-3][j][i].x;
					y_tt[k][j][i] = sp[0]*coors[k][j][i].y + sp[-1]*coors[k-1][j][i].y + sp[-2]*coors[k-2][j][i].y + sp[-3]*coors[k-3][j][i].y;
					z_tt[k][j][i] = sp[0]*coors[k][j][i].z + sp[-1]*coors[k-1][j][i].z + sp[-2]*coors[k-2][j][i].z + sp[-3]*coors[k-3][j][i].z;
				}
			}
		}
	}

	for (i = 0; i < 18; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	ierr = DMDAVecRestoreArray(cda, cv, &coors);CHKERRQ(ierr);

	return 0;
}


PetscErrorCode ef_metric_create(ef_metric **metptr, DM da, ef_fd *fd)
{
	PetscErrorCode ierr;
	int i;
	PetscInt dim;
	ef_metric *met = (ef_metric*) malloc(sizeof(ef_metric));

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	met->t2map[0][0] = 0;
	met->t2map[0][1] = 1;
	met->t2map[1][0] = 2;
	met->t2map[1][1] = 3;
	met->t2map[0][2] = 4;
	met->t2map[1][2] = 5;

	met->t3map[0][0] = 0;
	met->t3map[0][1] = 1;
	met->t3map[0][2] = 2;
	met->t3map[1][0] = 3;
	met->t3map[1][1] = 4;
	met->t3map[1][2] = 5;
	met->t3map[2][0] = 6;
	met->t3map[2][1] = 7;
	met->t3map[2][2] = 8;
	met->t3map[0][3] = 9;
	met->t3map[1][3] = 10;
	met->t3map[2][3] = 11;
	met->t3map[0][4] = 12;
	met->t3map[1][4] = 13;
	met->t3map[2][4] = 14;
	met->t3map[0][5] = 15;
	met->t3map[1][5] = 16;
	met->t3map[2][5] = 17;

	ierr = DMCreateGlobalVector(da, &met->jac[0]);CHKERRQ(ierr);
	ierr = VecDuplicate(met->jac[0], &met->jac2[0]);CHKERRQ(ierr);

	if (dim == 2) {
		for (i = 1; i < 4; i++) {
			ierr = VecDuplicate(met->jac[0], &met->jac[i]);CHKERRQ(ierr);
		}

		for (i = 1; i < 6; i++) {
			ierr = VecDuplicate(met->jac[0], &met->jac2[i]);CHKERRQ(ierr);
		}

		ierr = compute_2d(met, da, fd);CHKERRQ(ierr);
	} else if (dim == 3) {
		for (i = 1; i < 9; i++) {
			ierr = VecDuplicate(met->jac[0], &met->jac[i]);CHKERRQ(ierr);
		}

		for (i = 1; i < 18; i++) {
			ierr = VecDuplicate(met->jac[0], &met->jac2[i]);CHKERRQ(ierr);
		}

		ierr = compute_3d(met, da, fd);CHKERRQ(ierr);
	}

	*metptr = met;

	return 0;
}


PetscErrorCode ef_metric_destroy(ef_metric *metric)
{
	PetscErrorCode ierr;
	int i;

	for (i = 0; i < 4; i++) {
		ierr = VecDestroy(&metric->jac[i]);CHKERRQ(ierr);
	}

	for (i = 0; i < 6; i++) {
		ierr = VecDestroy(&metric->jac2[i]);CHKERRQ(ierr);
	}

	return 0;
}

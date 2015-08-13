#include "ef_neumann.h"
#include "ef_formula.h"

/**
 * Second order discretization of Neumann boundary conditions.
 */
static PetscErrorCode apply_neumann(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,ngx,ngy;
	PetscInt level;
	PetscInt nins;
	PetscScalar u[5], v[4];
	MatStencil row, col[4];
	PetscScalar **eps, **scale;
	PetscScalar **jac[4], **jac2[6];
	PetscScalar **x_s, **x_t, **y_s, **y_t;
	PetscScalar **x_ss, **x_tt, **y_ss, **y_tt, **x_st, **y_st;
	double f_s[5], f_t[5], f_ss[5], f_tt[5], f_st[5];
	double acc;
	ef_metric *met;
	ef_patch *patch = bc->patch;

	enum dir {
		O = 0, N, S, E, W
	};

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	met = bc->levels[level].metric;

	ierr = DMDAVecGetArray(da, bc->levels[level].eps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, bc->levels[level].scale, &scale);CHKERRQ(ierr);

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

	for (i = 0; i < 5; i++) {
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

	for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
		for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {

			acc = 0;
			for (k = 1; k < 5; k++) {
				u[k] = eps[j][i]*FORMULA_2D;
				acc += u[k];
			}

			v[O] = -1 * acc;

			nins = 4;
			row.i = i; row.j = j;
			col[0].i = i; col[0].j = j;

			if (patch->norm_dir == -2) { // South
				if (i == 0 || i == ngx-1) { // corner
					nins = 3;
					v[1] = u[N] + u[N];
					v[2] = u[E] + u[W];
					col[1].i = i; col[1].j = j+1;

					if (i == 0) {
						col[2].i = i+1; col[2].j = j;
					} else {// (i == ngx-1)
						col[2].i = i-1; col[2].j = j;
					}
				} else {
					v[1] = u[W];
					col[1].i = i-1; col[1].j = j;

					v[2] = u[E];
					col[2].i = i+1; col[2].j = j;

					v[3] = u[N] + u[S];
					col[3].i = i; col[3].j = j+1;
				}
			} else if (patch->norm_dir == 2) { // North
				if (i == 0 || i == ngx - 1) { // corner
					nins = 3;
					v[1] = u[N] + u[S];
					v[2] = u[E] + u[W];
					col[1].i = i; col[1].j = j-1;

					if (i == 0) {
						col[2].i = i+1; col[2].j = j;
					} else { // (i == ngx-1)
						col[2].i = i-1; col[2].j = j;
					}
				} else {
					v[1] = u[W];
					col[1].i = i-1; col[1].j = j;

					v[2] = u[E];
					col[2].i = i+1; col[2].j = j;

					v[3] = u[N] + u[S];
					col[3].i = i; col[3].j = j-1;
				}
			} else if (patch->norm_dir == -1) { // West
				if (j == 0 || j == ngy-1) { // corner
					nins = 3;
					v[1] = u[E] + u[W];
					v[2] = u[N] + u[S];
					col[1].i = i+1; col[1].j = j;

					if ( j == 0) {
						col[2].i = i; col[2].j = j+1;
					} else { // j == ngy-1
						col[2].i = i; col[2].j = j-1;
					}
				} else {
					v[1] = u[E] + u[W];
					col[1].i = i+1; col[1].j = j;

					v[2] = u[S];
					col[2].i = i; col[2].j = j-1;

					v[3] = u[N];
					col[3].i = i; col[3].j = j+1;
				}
			} else if (patch->norm_dir == 1) { // East
				if (j == 0 || j == ngy-1) { // corner
					nins = 3;
					v[1] = u[W] + u[E];
					v[2] = u[N] + u[S];
					col[1].i = i-1; col[1].j = j;

					if (j == 0) {
						col[2].i = i; col[2].j = j+1;
					} else { // j == ngy-1
						col[2].i = i; col[2].j = j-1;
					}
				} else {
					v[1] = u[E] + u[W];
					col[1].i = i-1; col[1].j = j;

					v[2] = u[S];
					col[2].i = i; col[2].j = j-1;

					v[3] = u[N];
					col[3].i = i; col[3].j = j+1;
				}
			}


			for (k = 0; k < nins; k++){
				if (nins == 3)
					v[k] = .25 * v[k];
				else
					v[k] = .5 * v[k];
				v[k] = scale[j][i]*v[k];
			}
			ierr = MatSetValuesStencil(A, 1, &row, nins, col, v, INSERT_VALUES);CHKERRQ(ierr);
		}
	}

	ierr = DMDAVecRestoreArray(da, bc->levels[level].eps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].scale, &scale);CHKERRQ(ierr);
	for (i = 0; i < 4; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);

	}

	for (i = 0; i < 6; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	return 0;
}


/**
 * Second order discretization of Neumann boundary conditions.
 */
static PetscErrorCode apply_neumann_3d(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,l,ngx,ngy,ngz;
	PetscInt level;
	PetscInt nb;
	PetscScalar u[7], v[6];
	int on[7];
	MatStencil row, col[6];
	PetscScalar ***eps, ***scale;
	PetscScalar ***jac[9], ***jac2[18];
	PetscScalar ***x_r, ***x_s, ***x_t, ***y_r, ***y_s, ***y_t,
		        ***z_r, ***z_s, ***z_t;
	PetscScalar ***x_rr, ***x_ss, ***x_tt, ***y_rr, ***y_ss, ***y_tt,
		        ***z_rr, ***z_ss, ***z_tt, ***x_rs, ***x_rt, ***x_st,
		        ***y_rs, ***y_rt, ***y_st, ***z_rs, ***z_rt, ***z_st;
	double f_r[7], f_s[7], f_t[7], f_rr[7], f_ss[7], f_tt[7],
		f_rs[7], f_rt[7], f_st[7];
	double acc;
	ef_metric *met;
	ef_patch *patch = bc->patch;

	enum dir {
		O = 0, N, S, E, W, F, B
	};

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	met = bc->levels[level].metric;

	ierr = DMDAVecGetArray(da, bc->levels[level].eps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, bc->levels[level].scale, &scale);CHKERRQ(ierr);

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

	for (i = 0; i < 7; i++) f_r[i] = f_s[i] = f_t[i] = f_rr[i] = f_ss[i] = f_tt[i] = f_rs[i] = f_rt[i] = f_st[i] = 0;

	f_r[E] = .5; f_r[W] = -.5;
	f_s[N] = .5; f_s[S] = -.5;
	f_t[F] = .5; f_t[B] = -.5;
	f_rr[O] = -2; f_rr[E] = 1; f_rr[W] = 1;
	f_ss[O] = -2; f_ss[N] = 1; f_ss[S] = 1;
	f_tt[O] = -2; f_tt[F] = 1; f_tt[B] = 1;

	for (k = patch->corners[level].is[2]; k <= patch->corners[level].ie[2]; k++) {
		for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
			for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {
				acc = 0;
				for (l = 1; l < 7; l++) {
					u[l] = -1 * eps[k][j][i] * FORMULA_3D;
					acc += u[l];
					on[l] = 0;
				}

				// ended here, look up where patch corners are set

				row.i = i; row.j = j; row.k = k;
				col[0].i = i; col[0].j = j; col[0].k = k;
				v[O] = -1 * acc;
				nb = 1;

				if (j == 0) {
					v[nb] = u[N] + u[S];
					col[nb].i = i; col[nb].j = j+1; col[nb].k = k;
					nb++;
					on[N] = 1; on[S] = 1;
				}

				if (j == ngy -1) {
					v[nb] = u[N] + u[S];
					col[nb].i = i; col[nb].j = j-1; col[nb].k = k;
					nb++;
					on[N] = 1; on[S] = 1;
				}

				if (i == 0) {
					v[nb] = u[E] + u[W];
					col[nb].i = i+1; col[nb].j = j; col[nb].k = k;
					nb++;
					on[E] = 1; on[W] = 1;
				}

				if (i == ngx - 1) {
					v[nb] = u[E] + u[W];
					col[nb].i = i-1; col[nb].j = j; col[nb].k = k;
					nb++;
					on[E] = 1; on[W] = 1;
				}

				if (k == 0) {
					v[nb] = u[F] + u[B];
					col[nb].i = i; col[nb].j = j; col[nb].k = k+1;
					nb++;
					on[F] = 1; on[B] = 1;
				}

				if (k == ngz - 1) {
					v[nb] = u[F] + u[B];
					col[nb].i = i; col[nb].j = j; col[nb].k = k-1;
					nb++;
					on[F] = 1; on[B] = 1;
				}

				for (l = 1; l < 7; l++) {
					if (!on[l]) {
						v[nb] = u[l];
						col[nb].i = i; col[nb].j = j; col[nb].k = k;
						if (l == F) col[nb].k++;
						if (l == B) col[nb].k--;
						if (l == E) col[nb].i++;
						if (l == W) col[nb].i--;
						if (l == N) col[nb].j++;
						if (l == S) col[nb].j--;
						nb++;
					}
				}

				for (l = 0; l < nb; l++) {
					if (nb == 4)
						v[l] = .125*v[l];
					else if (nb == 5)
						v[l] = .25*v[l];
					else
						v[l] = .5*v[l];
					v[l] = scale[k][j][i] * v[l];
				}
				ierr = MatSetValuesStencil(A, 1, &row, nb, col, v, INSERT_VALUES);CHKERRQ(ierr);
			}
		}
	}

	ierr = DMDAVecRestoreArray(da, bc->levels[level].eps, &eps);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].scale, &scale);CHKERRQ(ierr);

	for (i = 0; i < 9; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac[i], &jac[i]);CHKERRQ(ierr);

	}

	for (i = 0; i < 18; i++) {
		ierr = DMDAVecRestoreArray(da, met->jac2[i], &jac2[i]);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode apply_neumann_rhs(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,xs,ys,xe,ye,ngx,ngy;
	PetscInt level;
	PetscScalar **scale;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);
	ys = patch->gis[1]-1; xs = patch->gis[0]-1;
	ye = patch->gie[1];   xe = patch->gie[0];

	ierr = DMDAVecGetArray(da, bc->levels[level].nscale, &scale);CHKERRQ(ierr);
	for (j = ys; j < ye; j++) {
		for (i = xs; i < xe; i++) {
			if (((patch->norm_dir == -2 || patch->norm_dir == 2) && (i == 0 || i == ngx-1)) ||
			    ((patch->norm_dir == -1 || patch->norm_dir == 1) && (j == 0 || j == ngy-1)))
				scale[j][i] = .25;
			else
				scale[j][i] = .5;
		}
	}
	ierr = DMDAVecRestoreArray(da, bc->levels[level].nscale, &scale);CHKERRQ(ierr);

	return 0;
}


static PetscErrorCode apply_neumann_rhs_3d(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,xs,ys,zs,xe,ye,ze,ngx,ngy,ngz;
	PetscInt level;
	PetscScalar ***scale;
	int nb;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);
	ys = patch->gis[1]-1; xs = patch->gis[0]-1; zs = patch->gis[2]-1;
	ye = patch->gie[1];   xe = patch->gie[0]; ze = patch->gie[2];

	ierr = DMDAVecGetArray(da, bc->levels[level].nscale, &scale);CHKERRQ(ierr);
	for (k = zs; k < ze; k++) {
		for (j = ys; j < ye; j++) {
			for (i = xs; i < xe; i++) {
				nb = (1 + (j == 0) + (j == ngy-1) + (i == 0) + (i == ngx - 1) +
				      (k == 0) + (k == ngz - 1));
				if (nb == 4)
					scale[k][j][i] = .125;
				else if (nb == 3)
					scale[k][j][i] = .25;
				else
					scale[k][j][i] = .5;
			}
		}
	}
	ierr = DMDAVecRestoreArray(da, bc->levels[level].nscale, &scale);CHKERRQ(ierr);

	return 0;
}


static PetscErrorCode apply_op(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt dim;

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	if (dim == 2) {
		ierr = apply_neumann(bc, A, da);CHKERRQ(ierr);
	} else if (dim == 3) {
		ierr = apply_neumann_3d(bc, A, da);CHKERRQ(ierr);
	}

	return 0;
}


static PetscErrorCode apply_rhs(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;

	PetscInt dim;

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	if (dim == 2) {
		ierr = apply_neumann_rhs(bc, da);CHKERRQ(ierr);
	} else {
		ierr = apply_neumann_rhs_3d(bc, da);CHKERRQ(ierr);
	}

	return 0;
}


PetscErrorCode ef_neumann_create(ef_bc *bc)
{
	bc->apply = &apply_op;
	bc->apply_rhs = &apply_rhs;

	return 0;
}

#include "ef_dirichlet.h"


static PetscErrorCode update_dirichlet_sym(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,cnt;
	PetscInt ngx, ngy;
	PetscScalar v[8];
	MatStencil row[8], col;
	PetscInt level;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	for (i = 0; i < 8; i++) v[i] = 0;
	for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
		for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {
			col.i = i; col.j = j;
			cnt = 0;

			if (i-1 < patch->corners[level].is[0] &&
				i-1 >= 0) {
				row[cnt].i = i-1; row[cnt].j = j;
				cnt++;
				if (j-1 < patch->corners[level].is[1] &&
				    j-1 >= 0) {
					row[cnt].i = i-1; row[cnt].j = j-1;
					cnt++;
				}
				if (j+1 > patch->corners[level].ie[1] &&
				    j+1 <= ngy-1) {
					row[cnt].i = i-1; row[cnt].j = j+1;
					cnt++;
				}
			}
			if (i+1 > patch->corners[level].ie[0] &&
				i+1 <= ngx-1) {
				row[cnt].i = i+1; row[cnt].j = j;
				cnt++;
				if (j-1 < patch->corners[level].is[1] &&
				    j-1 >= 0) {
					row[cnt].i = i+1; row[cnt].j = j-1;
					cnt++;
				}
				if (j+1 > patch->corners[level].ie[1] &&
				    j+1 <= ngy-1) {
					row[cnt].i = i+1; row[cnt].j = j+1;
					cnt++;
				}
			}
			if (j-1 < patch->corners[level].is[1] &&
			    j-1 >= 0) {
				row[cnt].i = i; row[cnt].j = j-1;
				cnt++;
			}
			if (j+1 > patch->corners[level].ie[1] &&
				j+1 <= ngy-1) {
				row[cnt].i = i; row[cnt].j = j+1;
				cnt++;
			}

			ierr = MatSetValuesStencil(A, cnt, row, 1, &col, v, INSERT_VALUES);CHKERRQ(ierr);
		}
	}

	return 0;
}


static PetscErrorCode update_dirichlet_sym_3d(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,cnt;
	PetscInt ngx, ngy, ngz;
	PetscScalar v[6];
	MatStencil row[6], col;
	PetscInt level;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	for (i = 0; i < 6; i++) v[i] = 0;
	for (k = patch->corners[level].is[2]; k <= patch->corners[level].ie[2]; k++) {
		for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
			for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {
				col.i = i; col.j = j; col.k = k;
				cnt = 0;

				if (i-1 < patch->corners[level].is[0] &&
				    i-1 >= 0) {
					row[cnt].i = i-1; row[cnt].j = j; row[cnt].k = k;
					cnt++;
				}
				if (i+1 > patch->corners[level].ie[0] &&
				    i+1 <= ngx-1) {
					row[cnt].i = i+1; row[cnt].j = j; row[cnt].k = k;
					cnt++;
				}
				if (j-1 < patch->corners[level].is[1] &&
				    j-1 >= 0) {
					row[cnt].i = i; row[cnt].j = j-1; row[cnt].k = k;
					cnt++;
				}
				if (j+1 > patch->corners[level].ie[1] &&
				    j+1 <= ngy-1) {
					row[cnt].i = i; row[cnt].j = j+1; row[cnt].k = k;
					cnt++;
				}
				if (k-1 < patch->corners[level].is[2] &&
				    k-1 >= 0) {
					row[cnt].i = i; row[cnt].j = j; row[cnt].k = k-1;
					cnt++;
				}
				if (k+1 > patch->corners[level].ie[2] &&
				    k+1 <= ngz-1) {
					row[cnt].i = i; row[cnt].j = j; row[cnt].k = k+1;
					cnt++;
				}

				ierr = MatSetValuesStencil(A, cnt, row, 1, &col, v, INSERT_VALUES);CHKERRQ(ierr);
			}
		}
	}

	return 0;
}


static PetscErrorCode apply_dirichlet(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,cnt;
	PetscInt ngx, ngy;
	PetscScalar v[5];
	MatStencil row, col[5];
	PetscInt level;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, 0,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	v[0] = 1.0;
	for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
		for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {
			row.i = i; row.j = j;
			col[0].i = i; col[0].j = j;
			cnt = 1;

			if (i != 0) {
				col[cnt].i = i-1; col[cnt].j = j;
				v[cnt] = 0; cnt++;
			}
			if (j != 0) {
				col[cnt].i = i; col[cnt].j = j-1;
				v[cnt] = 0; cnt++;
			}
			if (i != ngx - 1) {
				col[cnt].i = i+1; col[cnt].j = j;
				v[cnt] = 0; cnt++;
			}
			if (j != ngy - 1) {
				col[cnt].i = i; col[cnt].j = j+1;
				v[cnt] = 0; cnt++;
			}

			ierr = MatSetValuesStencil(A, 1, &row, cnt, col, v, INSERT_VALUES);CHKERRQ(ierr);
		}
	}

	return 0;
}


static PetscErrorCode apply_dirichlet_3d(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,cnt;
	PetscInt ngx, ngy, ngz;
	PetscScalar v[7];
	MatStencil row, col[7];
	PetscInt level;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ierr = DMDAGetInfo(da, 0, &ngx, &ngy, &ngz,0,0,0,0,0,0,0,0,0);CHKERRQ(ierr);

	v[0] = 1.0;
	for (k = patch->corners[level].is[2]; k <= patch->corners[level].ie[2]; k++) {
		for (j = patch->corners[level].is[1]; j <= patch->corners[level].ie[1]; j++) {
			for (i = patch->corners[level].is[0]; i <= patch->corners[level].ie[0]; i++) {
				row.i = i; row.j = j; row.k = k;
				col[0].i = i; col[0].j = j; col[0].k = k;
				cnt = 1;

				if (i != 0) {
					col[cnt].i = i-1; col[cnt].j = j; col[cnt].k = k;
					v[cnt] = 0; cnt++;
				}
				if (j != 0) {
					col[cnt].i = i; col[cnt].j = j-1; col[cnt].k = k;
					v[cnt] = 0; cnt++;
				}
				if (k != 0) {
					col[cnt].i = i; col[cnt].j = j; col[cnt].k = k-1;
					v[cnt] = 0; cnt++;
				}
				if (i != ngx - 1) {
					col[cnt].i = i+1; col[cnt].j = j; col[cnt].k = k;
					v[cnt] = 0; cnt++;
				}
				if (j != ngy - 1) {
					col[cnt].i = i; col[cnt].j = j+1; col[cnt].k = k;
					v[cnt] = 0; cnt++;
				}
				if (k != ngz - 1) {
					col[cnt].i = i; col[cnt].j = j; col[cnt].k = k+1;
					v[cnt] = 0; cnt++;
				}

				ierr = MatSetValuesStencil(A, 1, &row, cnt, col, v, INSERT_VALUES);CHKERRQ(ierr);
			}
		}
	}

	return 0;
}




static PetscErrorCode apply_dirichlet_rhs(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,xs,ys,xe,ye,ym,xm;
	int stride[2];
	PetscScalar **gvec, **gcomp;
	PetscInt level;
	double **dirichlet;
	ef_dmap *dmap;
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ys = patch->gis[1]-1; xs = patch->gis[0]-1;
	ye = patch->gie[1];   xe = patch->gie[0];
	ym = patch->gie[1] - patch->gis[1] + 1;
	xm = patch->gie[0] - patch->gis[0] + 1;

	stride[0] = xm;
	stride[1] = ym;
	dmap = ef_dmap_create_2d(xs, ys, xm, ym, stride);
	ierr = ef_dmap_get(dmap, patch->dirichlet, &dirichlet);CHKERRQ(ierr);

	ierr = DMDAVecGetArray(da, bc->levels[level].g, &gvec);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, bc->levels[level].gcomp, &gcomp);CHKERRQ(ierr);
	for (j = ys; j < ye; j++) {
		for (i = xs; i < xe; i++) {
			gvec[j][i] = dirichlet[j][i];
			gcomp[j][i] = 0.0;
		}
	}
	ierr = ef_dmap_restore(dmap, &dirichlet);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].g, &gvec);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].gcomp, &gcomp);CHKERRQ(ierr);


	ef_dmap_destroy(dmap); free(dmap);

	return 0;
}


static PetscErrorCode apply_dirichlet_rhs_3d(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;
	PetscInt i,j,k,xs,ys,zs,xe,ye,ze,ym,xm,zm;
	PetscInt level;
	PetscScalar ***gvec, ***gcomp;
	double ***dirichlet;
	ef_dmap *dmap;
	int stride[3];
	ef_patch *patch = bc->patch;

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);

	ys = patch->gis[1]-1; xs = patch->gis[0]-1; zs = patch->gis[2]-1;
	ye = patch->gie[1];   xe = patch->gie[0]; ze = patch->gie[2];
	ym = patch->gie[1] - patch->gis[1] + 1;
	xm = patch->gie[0] - patch->gis[0] + 1;
	zm = patch->gie[2] - patch->gis[2] + 1;
	stride[0] = xm; stride[1] = ym; stride[2] = zm;

	dmap = ef_dmap_create_3d(xs, ys, zs, xm, ym, zm, stride);
	ierr = ef_dmap_get(dmap, patch->dirichlet, &dirichlet);CHKERRQ(ierr);

	ierr = DMDAVecGetArray(da, bc->levels[level].g, &gvec);CHKERRQ(ierr);
	ierr = DMDAVecGetArray(da, bc->levels[level].gcomp, &gcomp);CHKERRQ(ierr);
	for (k = zs; k < ze; k++) {
		for (j = ys; j < ye; j++) {
			for (i = xs; i < xe; i++) {
				gvec[k][j][i] = dirichlet[k][j][i];
				gcomp[k][j][i] = 0.0;
			}
		}
	}
	ierr = ef_dmap_restore(dmap, &dirichlet);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].g, &gvec);CHKERRQ(ierr);
	ierr = DMDAVecRestoreArray(da, bc->levels[level].gcomp, &gcomp);CHKERRQ(ierr);


	ef_dmap_destroy(dmap); free(dmap);

	return 0;
}


static PetscErrorCode apply_op(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt dim;

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	if (dim == 2) {
		ierr = apply_dirichlet(bc, A, da);CHKERRQ(ierr);
	} else if (dim == 3) {
		ierr = apply_dirichlet_3d(bc, A, da);CHKERRQ(ierr);
	}

	return 0;
}


static PetscErrorCode symmetric(ef_bc *bc, Mat A, DM da)
{
	PetscErrorCode ierr;
	PetscInt dim;

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	if (dim == 2) {
		ierr = update_dirichlet_sym(bc, A, da);CHKERRQ(ierr);
	} else if (dim == 3) {
		ierr = update_dirichlet_sym_3d(bc, A, da);CHKERRQ(ierr);
	}

	return 0;
}


static PetscErrorCode apply_rhs(ef_bc *bc, DM da)
{
	PetscErrorCode ierr;
	PetscInt dim;
	PetscInt level;

	ierr = DMDAGetInfo(da, &dim, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);CHKERRQ(ierr);

	ierr = DMGetCoarsenLevel(da, &level);CHKERRQ(ierr);
	if (!level) {
		if (dim == 2) {
			ierr = apply_dirichlet_rhs(bc, da);CHKERRQ(ierr);
		} else if (dim == 3) {
			ierr = apply_dirichlet_rhs_3d(bc, da);CHKERRQ(ierr);
		}
	}

	return 0;
}


PetscErrorCode ef_dirichlet_create(ef_bc *bc)
{
	bc->apply = &apply_op;
	bc->apply_rhs = &apply_rhs;
	bc->symmetric = &symmetric;

	return 0;
}

#include "residual.h"

	extern "C" {
		void bmg2_symstd_residual(int*,double*,double*,double*,double*,int*,int*,
								  int*,int*,int*,int*,int*,int*,int*);
	}

namespace boxmg { namespace bmg2d { namespace kernel {


namespace impls
{
	using namespace boxmg::bmg2d;

	void residual(const core::StencilOp & A, const core::GridFunc & x,
				  const core::GridFunc & b, core::GridFunc &r)
	{
		using namespace boxmg::bmg2d::core;

		const GridStencil &so = A.stencil();

		for (auto j: r.range(1)) {
			for (auto i: r.range(0)) {
				r(i,j) = (b(i,j) +
				          (so(i,j,Dir::W)  * x(i-1, j  ) +
				           so(i,j,Dir::E)  * x(i+1, j  ) +
				           so(i,j,Dir::S)  * x(i  , j-1) +
				           so(i,j,Dir::N)  * x(i  , j+1) +
						   so(i,j,Dir::SW) * x(i-1, j-1) +
				           so(i,j,Dir::SE) * x(i+1, j-1) +
				           so(i,j,Dir::NW) * x(i-1, j+1) +
				           so(i,j,Dir::NE) * x(i+1, j+1) -
						   so(i,j,Dir::C)  * x(i  , j)));
			}
		}
	}

	void residual_fortran(const core::StencilOp &A, const core::GridFunc &x,
						  const core::GridFunc &b, core::GridFunc &r)
	{
		int k = 0;
		int kf = 0;
		int ifd = 0;
		int nstncl = 5;
		int ibc = 0;
		int irelax = 0;
		int irelax_sym = 0;
		int updown = 0;
		int ii = r.len(0);
		int jj = r.len(1);

		core::StencilOp &Ad = const_cast<core::StencilOp&>(A);
		core::GridFunc &xd = const_cast<core::GridFunc&>(x);
		core::GridFunc &bd = const_cast<core::GridFunc&>(b);
		using namespace boxmg::bmg2d::core;

		bmg2_symstd_residual(&k, Ad.data(), bd.data(), xd.data(), r.data(), &ii, &jj,
							 &kf, &ifd, &nstncl, &ibc, &irelax, &irelax_sym, &updown);

	}
}


}}}

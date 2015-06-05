#include <immintrin.h>

typedef float v4sf __attribute__ ((vector_size (16)));

void saxpy(const int n, const float alpha, const float * x, float * y) {
	int i;

	for (i=0; i<n; ++i) {
		y[i] = alpha*x[i] + y[i];
	} // for
} // saxpy

void saxpy_v4sf(const int n, const float alpha, const v4sf * x, v4sf * y) {
	int i;
	v4sf a = {alpha, alpha, alpha, alpha};

	for (i=0; i<n; ++i) {
		y[i] = a*x[i] + y[i];
	} // for
} // saxpy_v4sf

#if 0
void saxpy_v4sf_fmadd(const int n, const float alpha,
	const v4sf * x, v4sf * y) {
int i;
v4sf a = {alpha, alpha, alpha, alpha};

for (i=0; i<n; ++i) {
    y[i] = __builtin_ia32_fmaddps(a, x[i], y[i]);
//		y[i] = _mm_fmadd_ps(a, x[i], y[i]);
//		y[i] = __builtin_ia32_haddps(a, a);
	} // for
} // saxpy_v4sf_fmadd

void saxpy_v4sf_unroll2(const int n, const float alpha,
	const float * x, float * y) {
	int i;

	for(i=0; i<n/2; i+=2) {
		const float m0 = alpha*x[i];
		const float m1 = alpha*x[i+1];
		y[i] = m0 + y[i];
		y[i+1] = m1 + y[i+1];
	} // for
} // saxpy_v4sf_unroll2
#endif

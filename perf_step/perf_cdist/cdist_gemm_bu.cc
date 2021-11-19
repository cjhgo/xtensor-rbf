#include "cdist.h"


#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define C(i,j,ldc) C[ (j)*ldc + (i)]

#define mc 256
#define kc 32 
#define nc 128


void kernel_4x4(int k, double * A, int lda , double * B, int ldb, double *C, int ldc){

	for(int p = 0; p < k; p++){


		C(0, 0, ldc) += l2_dist( A(0, p, lda), B(p, 0, ldb)); 
		C(0, 1, ldc) += l2_dist( A(0, p, lda), B(p, 1, ldb)); 
		C(0, 2, ldc) += l2_dist( A(0, p, lda), B(p, 2, ldb)); 
		C(0, 3, ldc) += l2_dist( A(0, p, lda), B(p, 3, ldb)); 



		C(1, 0, ldc) += l2_dist( A(1, p, lda), B(p, 0, ldb)); 
		C(1, 1, ldc) += l2_dist( A(1, p, lda), B(p, 1, ldb)); 
		C(1, 2, ldc) += l2_dist( A(1, p, lda), B(p, 2, ldb)); 
		C(1, 3, ldc) += l2_dist( A(1, p, lda), B(p, 3, ldb)); 


		C(2, 0, ldc) += l2_dist( A(2, p, lda), B(p, 0, ldb)); 
		C(2, 1, ldc) += l2_dist( A(2, p, lda), B(p, 1, ldb)); 
		C(2, 2, ldc) += l2_dist( A(2, p, lda), B(p, 2, ldb)); 
		C(2, 3, ldc) += l2_dist( A(2, p, lda), B(p, 3, ldb)); 

		C(3, 0, ldc) += l2_dist( A(3, p, lda), B(p, 0, ldb)); 
		C(3, 1, ldc) += l2_dist( A(3, p, lda), B(p, 1, ldb)); 
		C(3, 2, ldc) += l2_dist( A(3, p, lda), B(p, 2, ldb)); 
		C(3, 3, ldc) += l2_dist( A(3, p, lda), B(p, 3, ldb)); 


	}
}
/**
 * @brief 
 * 
 */
void unroll_cdist(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	for(int i = 0; i < n; i += 4)
	{
		for(int j = 0; j < m; j += 4)
		{	
			kernel_4x4(k, &A(j, 0, lda), lda, &B(0, i, ldb), ldb,  &C(j, i, ldc), ldc);
		}
	}
}

/**
 * @brief 
 * 基于分块 和 分块儿内循环展开 来计算 
 * @param A (m, k)
 * @param B (k, n)
 * @param C (m, n)
 */
void cdist_gemm_BU(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	bp(cdist_gemm_BU);

	
	for(int np = 0; np < n; np += nc)
	{int pn_p = std::min(nc, n - np);

		for(int mi = 0; mi < m; mi += mc)
		{int mi_m = std::min(mc, m-mi);

			for(int kj = 0; kj < k; kj += kc)
			{int kj_k = std::min(kc,  k-kj);

				double * Amk = &A(mi, kj, m);//mi_m, kj_k
				double * Bkn = &B(kj, np, k);//kj_k, pn_p
				double * Cmn = &C(mi, np, m);
				unroll_cdist(mi_m, pn_p, kj_k, Amk, lda, Bkn, ldb, Cmn, ldc);
			}
		}
	}

	ep(cdist_gemm_BU);
}

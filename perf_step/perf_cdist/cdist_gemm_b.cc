#include "cdist.h"


#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define C(i,j,ldc) C[ (j)*ldc + (i)]

#define mc 256
#define kc 32 
#define nc 128

/**
 * @brief 
 * ToDo: Unification with cdist_forloop_nkm ...
 */
void blocking_cdist(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	for(int i = 0; i < n; i++){
		for(int j = 0; j < k; j++){
			for(int p = 0; p < m; p++){
				double a_p_j = A(p, j, lda);
				double b_j_i = B(j, i, ldb);
				double delta = a_p_j - b_j_i;
				C(p, i, ldc) +=  delta * delta;
			}
		}
	}
}

/**
 * @brief 
 * 基于分块计算 
 * @param A (m, k)
 * @param B (k, n)
 * @param C (m, n)
 */
void cdist_gemm_block(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	bp(cdist_gemm_block);

	
	for(int np = 0; np < n; np += nc)
	{int pn_p = std::min(nc, n - np);

		for(int mi = 0; mi < m; mi += mc)
		{int mi_m = std::min(mc, m-mi);

			for(int kj = 0; kj < k; kj += kc)
			{int kj_k = std::min(kc,  k-kj);

				double * Amk = &A(mi, kj, m);//mi_m, kj_k
				double * Bkn = &B(kj, np, k);//kj_k, pn_p
				double * Cmn = &C(mi, np, m);
				// printf("will compute cdist on (m,k), (k, n) : %d %d %d %d \n", mi, kj, kj, np);
				// printf("will compute cdist with len m,k,n : %d %d %d\n", mi_m, kj_k, pn_p);
				// blocking_cdist(Amk, mi_m, m, Bkn, pn_p, k, kj_k, Cmn, m);
				blocking_cdist(mi_m, pn_p, kj_k, Amk, lda, Bkn, ldb, Cmn, ldc);
				// std::cout << std::endl << std::endl;
			}
		}
	}

	ep(cdist_gemm_block);
}

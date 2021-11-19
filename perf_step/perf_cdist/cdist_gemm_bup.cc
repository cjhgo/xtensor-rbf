#include "cdist.h"


#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define Brow(i, j, ldb) B[ (i)*ldb + j]
#define C(i,j,ldc) C[ (j)*ldc + (i)]

#define mc 256
#define kc 64 
#define nc 128


void pack_matrix_A(int k, double *A, int lda, double * to){
	int index = 0;
	for(int i = 0; i < k; i++){
		to[index++] = A(0, i, lda);
		to[index++] = A(1, i, lda);
		to[index++] = A(2, i, lda);
		to[index++] = A(3, i, lda);
	}
}

void pack_matrix_B(int k, double *B, int ldb, double * to){
	int index = 0;
	for(int i = 0; i < k; i++){
		to[index++] = B(i, 0, ldb);
		to[index++] = B(i, 1, ldb);
		to[index++] = B(i, 2, ldb);
		to[index++] = B(i, 3, ldb);
	}
}

void packed_kernel_4x4(int k, double * A, double * B, double *C, int ldc){

	for(int p = 0; p < k; p++){


		C(0, 0, ldc) += l2_dist( A[4*p], B[4*p+0]); 
		C(0, 1, ldc) += l2_dist( A[4*p], B[4*p+1]); 
		C(0, 2, ldc) += l2_dist( A[4*p], B[4*p+2]); 
		C(0, 3, ldc) += l2_dist( A[4*p], B[4*p+3]); 



		C(1, 0, ldc) += l2_dist( A[4*p+1], B[4*p+0]); 
		C(1, 1, ldc) += l2_dist( A[4*p+1], B[4*p+1]); 
		C(1, 2, ldc) += l2_dist( A[4*p+1], B[4*p+2]); 
		C(1, 3, ldc) += l2_dist( A[4*p+1], B[4*p+3]); 


		C(2, 0, ldc) += l2_dist( A[4*p+2], B[4*p+0]); 
		C(2, 1, ldc) += l2_dist( A[4*p+2], B[4*p+1]); 
		C(2, 2, ldc) += l2_dist( A[4*p+2], B[4*p+2]); 
		C(2, 3, ldc) += l2_dist( A[4*p+2], B[4*p+3]); 

		C(3, 0, ldc) += l2_dist( A[4*p+3], B[4*p+0]); 
		C(3, 1, ldc) += l2_dist( A[4*p+3], B[4*p+1]); 
		C(3, 2, ldc) += l2_dist( A[4*p+3], B[4*p+2]); 
		C(3, 3, ldc) += l2_dist( A[4*p+3], B[4*p+3]); 

	}
}

/**
 * @brief 
 * 
 */
void unroll_pack_cdist(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	double packedA[4*k];
	double packedB[4*k];

	for(int i = 0; i < n; i += 4)
	{
		for(int j = 0; j < m; j += 4)
		{	
			pack_matrix_A(k, &A(j, 0, lda), lda, packedA);
			pack_matrix_B(k, &B(0, i, ldb), ldb, packedB);
			packed_kernel_4x4(k, packedA, packedB, &C(j, i, ldc), ldc);
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
void cdist_gemm_BUP(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	bp(cdist_gemm_BUP);

	
	for(int np = 0; np < n; np += nc)
	{int pn_p = std::min(nc, n - np);

		for(int mi = 0; mi < m; mi += mc)
		{int mi_m = std::min(mc, m-mi);

			for(int kj = 0; kj < k; kj += kc)
			{int kj_k = std::min(kc,  k-kj);

				double * Amk = &A(mi, kj, m);//mi_m, kj_k
				double * Bkn = &B(kj, np, k);//kj_k, pn_p
				double * Cmn = &C(mi, np, m);
				unroll_pack_cdist(mi_m, pn_p, kj_k, Amk, lda, Bkn, ldb, Cmn, ldc);
			}
		}
	}

	ep(cdist_gemm_BUP);
}

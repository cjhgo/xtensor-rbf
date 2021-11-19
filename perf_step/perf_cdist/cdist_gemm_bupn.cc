#include "cdist.h"


#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define Brow(i, j, ldb) B[ (i)*ldb + j]
#define C(i,j,ldc) C[ (j)*ldc + (i)]

#define mc 256
#define kc 96 
#define nc 128


void pack_matrix_A(int k, double *A, int lda, double * to);

void pack_matrix_B(int k, double *B, int ldb, double * to);

void packed_neon_kernel_4x4(int k, double * A, double * B, double *C, int ldc){

	double cres [16] = {0};
	double * cbase = &cres[0];
	int p = k;

	asm volatile(
		"eor v14.8b, v0.8b, v0.8b 										     \n"
		"eor v15.8b, v1.8b, v1.8b                         \n"
		"eor v16.8b, v2.8b, v2.8b                         \n"
		"eor v17.8b, v3.8b, v3.8b                         \n"
		"eor v18.8b, v4.8b, v4.8b                         \n"
		"eor v19.8b, v5.8b, v5.8b                         \n"
		"eor v20.8b, v0.8b, v0.8b                         \n"
		"eor v21.8b, v1.8b, v1.8b                         \n"


		"0:																\n\t"
		"ld1 {v0.2d}, [%[acol_ptr]], #0x10 \n\t"
		"ld1 {v1.2d}, [%[acol_ptr]], #0x10 \n\t"

		"ld1r {v2.2d}, [%[brow_ptr]], #0x8\n\t"
		"ld1r {v3.2d}, [%[brow_ptr]], #0x8\n\t"
		"ld1r {v4.2d}, [%[brow_ptr]], #0x8\n\t"
		"ld1r {v5.2d}, [%[brow_ptr]], #0x8\n\t"


		"fsub v6.2d, v0.2d, v2.2d \n\t"
		"fsub v7.2d, v1.2d, v2.2d \n\t"
		"fsub v8.2d, v0.2d, v3.2d \n\t"
		"fsub v9.2d, v1.2d, v3.2d \n\t"
		"fsub v10.2d, v0.2d, v4.2d \n\t"
		"fsub v11.2d, v1.2d, v4.2d \n\t"
		"fsub v12.2d, v0.2d, v5.2d \n\t"
		"fsub v13.2d, v1.2d, v5.2d \n\t"

		"subs %[p], %[p], #1			 \n\t"
		"fmla  v14.2d,  v6.2d,  v6.2d \n\t"
		"fmla  v15.2d,  v7.2d,  v7.2d \n\t"
		"fmla  v16.2d,  v8.2d,  v8.2d \n\t"
		"fmla  v17.2d,  v9.2d,  v9.2d \n\t"
		"fmla  v18.2d, v10.2d, v10.2d \n\t"
		"fmla  v19.2d, v11.2d, v11.2d \n\t"
		"fmla  v20.2d, v12.2d, v12.2d \n\t"
		"fmla  v21.2d, v13.2d, v13.2d \n\t"

			
		"bne 0b											 \n\t"

		"st1 {v14.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v15.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v16.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v17.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v18.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v19.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v20.2d}, [%[cres_ptr]], #0x10\n\t"
		"st1 {v21.2d}, [%[cres_ptr]], #0x10\n\t"

		: [cres_ptr] "+r" (cbase),
		[acol_ptr] "+r" (A), [brow_ptr] "+r" (B),
		[p] "+r"(p)
		:
		: "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8",
			"v9", "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18",
			"v19", "v20", "v21"
	);

	for(int j = 0; j < 4; j++){
	for(int i = 0; i < 4; i++){
			C(i,j, ldc) += cres[j*4+i];
			// printf(" at %d %d  c is %.3f my is %.3f\n",i, j,  C(i,j, ldc), cres[j*4+i]);
		}
	}

}

/**
 * @brief 
 * 
 */
void unroll_pack_neon_cdist(int m, int n, int k, double * A, int lda,
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
			packed_neon_kernel_4x4(k, packedA, packedB, &C(j, i, ldc), ldc);
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
void cdist_gemm_BUPN(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	bp(cdist_gemm_BUPN);

	
	for(int np = 0; np < n; np += nc)
	{int pn_p = std::min(nc, n - np);

		for(int mi = 0; mi < m; mi += mc)
		{int mi_m = std::min(mc, m-mi);

			for(int kj = 0; kj < k; kj += kc)
			{int kj_k = std::min(kc,  k-kj);

				double * Amk = &A(mi, kj, m);//mi_m, kj_k
				double * Bkn = &B(kj, np, k);//kj_k, pn_p
				double * Cmn = &C(mi, np, m);
				unroll_pack_neon_cdist(mi_m, pn_p, kj_k, Amk, lda, Bkn, ldb, Cmn, ldc);
			}
		}
	}

	ep(cdist_gemm_BUPN);
}

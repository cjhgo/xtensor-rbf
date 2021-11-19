#include "util.h"

#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define C(i,j,ldc) C[ (j)*ldc + (i)]

void cdist_forloop_mkn(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{



};

/**
 * @brief 
 * 朴素的 for-loop 计算
 * @param A (k,m)
 * @param B (k,n)
 * @param C (m,n)
 */
void cdist_forloop_mnk(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc)
{
	bp(cdist_forloop_mnk);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){	
			for(int p = 0; p < k; p++){
				double a_p_i = A(p, i, lda);
				double b_p_j = B(p, j, ldb);
				double delta = a_p_i - b_p_j;
				C(i,j, ldc) += delta * delta;
			}
		}
	}
	ep(cdist_forloop_mnk);

}


void cdist_forloop_nmk(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_nkm(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_kmn(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_knm(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);
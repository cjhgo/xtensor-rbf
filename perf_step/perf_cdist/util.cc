#include "util.h"

#define A(i,j,lda) A[ (j)*lda + (i)]
#define B(i,j,ldb) B[ (j)*ldb + (i)]
#define C(i,j,ldc) C[ (j)*ldc + (i)]


/**
 * @brief 
 * 按列打印 矩阵 A 的元素
 * @param m 
 * @param n 
 * @param A 
 * @param lda 
 */
void print_matrix( int m, int n, double *A, int lda )
{
  int i, j;

  for ( j=0; j<n; j++ ){
    for ( i=0; i<m; i++ )
      printf("%.2f ", A( i,j, lda) );
    printf("\n");
  }
  printf("\n");
}

double compare_matrices(int m, int n, double *A, int lda, double *B, int ldb){
	double max_diff = 0.0;
  double diff;
	for(int j = 0; j < n; j++ )
		for(int i = 0; i < m; i++ ){
      diff = std::abs( A( i,j, lda) - B( i,j, ldb ));
			max_diff = (diff > max_diff ? diff : max_diff);
		}

	return max_diff;
}
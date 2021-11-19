
#include "util.h"


/**
 * @brief 
 * 基于二项式分解的思路来算 cdist
 * @param A (k,m)
 * @param B (k,n)
 * @param C (m,n)
 */
void cdist_eigen_bin_decom(MatrixXd& A, MatrixXd& B, MatrixXd& C){
	bp(cdist_eigen_bin_decom);
	C = ((-2 * A.transpose() * B).colwise() +
		A.colwise().squaredNorm().transpose())
		.rowwise() +
		B.colwise().squaredNorm();
	ep(cdist_eigen_bin_decom);
}


/**
 * @brief 
 * 一次求一列 C(:,i): A(m个列向量) 到 B(:,i) 的 dist;求 n 次
 * @param A  (k,m)
 * @param B  (k,n)
 * @param C  (m,n)
 */
void cdist_eigen_vwise(MatrixXd& A, MatrixXd& B, MatrixXd& C){
	bp(cdist_eigen_vwise);
	int m = A.cols();
	int n = B.cols();
	for(int i = 0; i < n ; i++){
		C.col(i) = (A.colwise()-B.col(i)).colwise().squaredNorm().transpose();
	}
	ep(cdist_eigen_vwise);
}
#include "util.h"
#include "cdist.h"
#include <iostream>

MatrixXd A,B;

int main(int argc, char const *argv[])
{

	A = MatrixXd(4,8);
	B = MatrixXd(4,4);

	A << 1,1,2,2,3,3,4,4,
			 1,1,2,2,3,3,4,4,
			 5,5,6,6,7,7,8,8,
			 5,5,6,6,7,7,8,8;

	B << 1,1,3,3,
			 1,1,3,3,
			 2,2,4,4,
			 2,2,4,4;
	
	MatrixXd C0 = MatrixXd::Zero(8,4);
	MatrixXd C1 = MatrixXd::Zero(8,4);
	MatrixXd C2 = MatrixXd::Zero(8,4);
	MatrixXd C3 = MatrixXd::Zero(8,4);
	MatrixXd C4 = MatrixXd::Zero(8,4);
	MatrixXd C5 = MatrixXd::Zero(8,4);


	cdist_eigen_bin_decom(A, B, C0);
	cdist_eigen_vwise(A, B, C1);
	cdist_forloop_mnk(8, 4, 4, A.data(), 4, 
													 B.data(), 4,
													 C2.data(), 8);

	MatrixXd At = A.transpose();

	cdist_gemm_BU(8, 4, 4, At.data(), 8, 
														B.data(), 4,
														C3.data(), 8);

	cdist_gemm_BUP(8, 4, 4, At.data(), 8, 
														B.data(), 4,
														C4.data(), 8);

	cdist_gemm_BUPN(8, 4, 4, At.data(), 8, 
														B.data(), 4,
														C5.data(), 8);
	std::cout << A 
	<< std::endl << std::endl;

	std::cout << B 
	<< std::endl << std::endl;

	std::cout << C0
	<< std::endl << std::endl;

	std::cout << C1 
	<< std::endl << std::endl;

	std::cout << C2 
	<< std::endl << std::endl;

	std::cout << C3 
	<< std::endl << std::endl;

	std::cout << C4 
	<< std::endl << std::endl;

	std::cout << C5 
	<< std::endl << std::endl;
	return 0;
}

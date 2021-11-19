#include <cstdlib>
#include "cdist.h"
#include "util.h"

void bench(int m, int n, int k){
	MatrixXd A = MatrixXd::Random(k, m);
	MatrixXd B = MatrixXd::Random(k, n);
	MatrixXd C0 = MatrixXd::Zero(m, n);
	MatrixXd C1 = MatrixXd::Zero(m, n);
	MatrixXd C2 = MatrixXd::Zero(m, n);
	MatrixXd C3 = MatrixXd::Zero(m, n);
	MatrixXd C4 = MatrixXd::Zero(m, n);
	MatrixXd C5 = MatrixXd::Zero(m, n);
	MatrixXd C6 = MatrixXd::Zero(m, n);

	printf("sum of C0 is %.3f\n", C0.sum());
	printf("sum of C1 is %.3f\n", C1.sum());
	printf("sum of C2 is %.3f\n", C2.sum());
	printf("sum of C3 is %.3f\n", C3.sum());
	printf("sum of C4 is %.3f\n", C4.sum());
	printf("sum of C5 is %.3f\n", C5.sum());
	printf("sum of C6 is %.3f\n", C6.sum());

	cdist_eigen_bin_decom(A, B, C0);

	cdist_eigen_vwise(A, B, C1);

	cdist_forloop_mnk(m, n, k, A.data(), k,
														 B.data(), k,
														 C2.data(), m);

	bp(matrix_transpose);
	MatrixXd At = A.transpose();
	ep(matrix_transpose);

	cdist_gemm_block(m, n, k, At.data(), m,
														B.data(), k,
														C3.data(), m);

	cdist_gemm_BU(m, n, k, At.data(), m,
														B.data(), k,
														C4.data(), m);

	cdist_gemm_BUP(m, n, k, At.data(), m,
														B.data(), k,
														C5.data(), m);

	cdist_gemm_BUPN(m, n, k, At.data(), m,
														B.data(), k,
														C6.data(), m);


	double diff01 = compare_matrices(m, n, C0.data(), m, C1.data(), m);
	double diff12 = compare_matrices(m, n, C1.data(), m, C2.data(), m);
	double diff23 = compare_matrices(m, n, C2.data(), m, C3.data(), m);
	double diff34 = compare_matrices(m, n, C3.data(), m, C4.data(), m);
	double diff45 = compare_matrices(m, n, C4.data(), m, C5.data(), m);
	double diff56 = compare_matrices(m, n, C5.data(), m, C6.data(), m);


	printf("sum of C0 is %.3f\n", C0.sum());
	printf("sum of C1 is %.3f\n", C1.sum());
	printf("sum of C2 is %.3f\n", C2.sum());
	printf("sum of C3 is %.3f\n", C3.sum());
	printf("sum of C4 is %.3f\n", C4.sum());
	printf("sum of C5 is %.3f\n", C5.sum());
	printf("sum of C6 is %.3f\n", C6.sum());

	printf("diff 0 1 is %.3f\n", diff01);
	printf("diff 1 2 is %.3f\n", diff12);
	printf("diff 2 3 is %.3f\n", diff23);
	printf("diff 3 4 is %.3f\n", diff34);
	printf("diff 4 5 is %.3f\n", diff45);
	printf("diff 5 6 is %.3f\n", diff56);

}
int main(int argc, char const *argv[])
{
	if( argc < 4){
		std::cout << "usage: exe m n k" << std::endl;
		return 0;
	}

	// bench(768, 768, 2);
	// bench(768, 1, 256);
	// bench(10000, 1, 256);
	// bench(10000, 512, 256);
	// bench(1024, 1024, 512);
	int m = std::atoi(argv[1]);
	int n = std::atoi(argv[2]);
	int k = std::atoi(argv[3]);
	bench(m, n, k);

	// bench(5000, 5000, 256);
	// bench(600,  400,  256);
	// bench(2048, 2048, 1024);
	// bench(100, 100, 100);

	// for(int i = 400; i < 600; i += 20){
	// 	bench(i, i, i);
	// }
	return 0;
}

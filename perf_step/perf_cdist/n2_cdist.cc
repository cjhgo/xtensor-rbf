#include <iostream>
#include <fstream>
#include <chrono>

#include <cmath>
#include <Eigen/LU>
#include <vector>
#include <iostream>
#include <sstream>      // std::stringstream
#include <fstream>
#include <string>

using Eigen::PartialPivLU;
using Eigen::FullPivLU;
using Eigen::Map;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

#define bp(s) auto  begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s) std::cout << "Profile:  "  << #s << " spend time(s): " \
             << (std::chrono::high_resolution_clock::now() - begin_##s)*1.0 / std::chrono::seconds(1) \
             << std::endl << std::endl; 


             
MatrixXd cdist(const MatrixXd &meA, const MatrixXd &meB) {
  int m = meA.cols();
  int n = meB.cols();
  MatrixXd D(m, n);
  double * c = D.data();

  int index = 0;
  for(int i = 0; i < n; i++){
    double n0 = meB(0, i);
    double n1 = meB(1, i);
    for(int j = 0; j < m;j++){
      double m0 = meA(0, j);
      double m1 = meA(1, j);

      double diff0 = m0 - n0;
      double diff1 = m1 - n1;
      c[index++] = diff0*diff0 + diff1*diff1;
    }
  }
  return D;
}

MatrixXd mdist(const MatrixXd& meA)
{


	size_t dims = meA.cols();
	MatrixXd mm = meA.transpose() * meA;
	VectorXd diag = mm.diagonal();
	for(size_t i = 0; i < dims; i++){
		for(size_t j = 0; j < dims; j++){
			mm(i,j) = diag(i) + diag(j) - 2*mm(i,j);
			// mm(j,i) = mm(i,j);
		}
	}


	// MatrixXd D = (
  //   (-2 * mm).colwise() + 
  //   mm.diagonal()
  //   ).rowwise() + 
  //   mm.diagonal().transpose();
  return mm;
}
MatrixXd cdist_eig(const MatrixXd& meA, const MatrixXd&  meB)
{

  // MatrixXd D( A.cols(), B.cols());

	MatrixXd D = (
    (-2 * meA.transpose() * meB).colwise() + 
    meA.colwise().squaredNorm().transpose()
    ).rowwise() + 
    meB.colwise().squaredNorm();

  return D;
}


void bench(int row, int col)
{



  MatrixXd bigb  = MatrixXd::Random(row,col);

  bp(cdist_eig1)
  cdist_eig(bigb,bigb);
  ep(cdist_eig1)

  bp(mdistt)
  // mdist(bigb);
  cdist(bigb, bigb);
  ep(mdistt)

}


int main(int argc, char* argv[])
{

  Matrix<double, 2,3> A;
  A   << 1,2,3,3,1,5;
	std::cout << "A is " << A << std::endl;
  std::cout<< "dist eig is :\n" << cdist_eig(A,A) << std::endl;
  std::cout<< "dist eig is :\n" << cdist(A, A) << std::endl;


	for(int i = 0; i < 10; i++){
  	MatrixXd bigb  = MatrixXd::Random(3, i);
		mdist(bigb);
	}
  bench(100,2);
  for(int i  = 2; i < 4; i += 1)
  for(int j  = 300; j < 1100; j += 100)
  {
    std::cout << " run on size : " << i << "\t" << j << std::endl;
    bench(i,j);
  }

  return 0;
}
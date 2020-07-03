#include <iostream>
#include "xtensor/xarray.hpp"
#include <fstream>
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xmath.hpp"
#include "xtensor/xnorm.hpp"
#include "xtensor-blas/xlinalg.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor/xmanipulation.hpp"
#include "xtensor/xrandom.hpp"
#include "xtensor/xcsv.hpp"

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

#if defined(XTENSOR_USE_XSIMD)

namespace xs = xsimd;



#endif // XTENSOR_USE_XSIMD


#define bp(s) auto  begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s) std::cout << "Profile:  "  << #s << " spend time(s): " \
             << (std::chrono::high_resolution_clock::now() - begin_##s)*1.0 / std::chrono::seconds(1) \
             << std::endl << std::endl; 
using xt::xarray;


             
MatrixXd cdist_eig(const MatrixXd& meA, const MatrixXd&  meB)
{

  // MatrixXd D( A.cols(), B.cols());

	MatrixXd D = (
    (-2 * meA.transpose() * meB).colwise() + 
    meA.colwise().squaredNorm().transpose()
    ).rowwise() + 
    meB.colwise().squaredNorm();

  return D.array().pow(0.5);
}

auto cdist_xt(const xt::xarray<double>& A, const xt::xarray<double>& B)
{
//   auto A_shape = A.shape();
//   auto B_shape = B.shape();

  xarray<double> A_sq =  xt::norm_sq(A,{0});
  xarray<double> B_sq =  xt::norm_sq(B,{0});

  auto AdotB = xt::linalg::dot(xt::transpose(A),B);

  A_sq.reshape({A.shape(1),1});
  B_sq.reshape({1,B.shape(1)});

  xarray<double> dist = xt::sqrt(A_sq -2*AdotB + B_sq);

  return dist;
}

void bench(int row, int col)
{

  xarray<double> biga = xt::random::randn<double>({row, col});

  bp(cdist_xt1)
  cdist_xt(biga,biga);
  ep(cdist_xt1)


  MatrixXd bigb  = MatrixXd::Random(row,col);

  bp(cdist_eig1)
  cdist_eig(bigb,bigb);
  ep(cdist_eig1)


}


void bench2(int row, int col)
{

  xarray<double> biga = xt::random::randn<double>({row, col});

  bp(cdist_xt2)
  cdist_xt(biga,biga);
  ep(cdist_xt2)

  MatrixXd bigb  = MatrixXd::Zero(row,col);
  for(size_t i = 0; i < row; i++)
  {
    for(size_t j = 0; j < col; j++)
    {
      bigb(i,j) = biga(i,j);
    }
  }

  bp(cdist_eig2)
  cdist_eig(bigb,bigb);
  ep(cdist_eig2)


}
int main(int argc, char* argv[])
{
  xt::xarray<double> a = {
  {1,2,3},
  {3,1,5}};

  cdist_xt(a,a);
  std::cout << "dist xt is :\n" << cdist_xt(a,a) << std::endl;

  Matrix<double, 2,3> A;
  A   << 1,2,3,3,1,5;
  std::cout<< "dist eig is :\n" << cdist_eig(A,A) << std::endl;


  bench2(100,2);
  for(int i  = 10; i < 200; i += 20)
  for(int j  = 100; j < 50000; j += 2000)
  {
    std::cout << " run on size : " << i << "\t" << j << std::endl;
    bench2(i,j);
  }

  return 0;
}
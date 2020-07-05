#include "rbf_xt.h"

xt::xarray<double> 
cdist(const xt::xarray<double>& A, const xt::xarray<double>& B)
{

  bp(norm_sqa)
  xarray<double> A_sq =  xt::norm_sq(A,{0});
  ep(norm_sqa)

  bp(norm_sqb)
  xarray<double> B_sq =  xt::norm_sq(B,{0});
  ep(norm_sqb)

  bp(dot)
  xarray<double> AdotB = xt::linalg::dot(xt::transpose(A),B);
  ep(dot)


  A_sq.reshape({A.shape(1),1});
  B_sq.reshape({1,B.shape(1)});

  bp(sqrt)
  xarray<double> dist = xt::sqrt(A_sq -2*AdotB + B_sq);
  ep(sqrt)

  return dist;
}

xt::xarray<double> 
thin_plate(xt::xarray<double>& input)
{

  bp(pow)
  xt::xarray<double> pow_r = xt::pow(input,2);
  ep(pow)

  bp(log)
  xt::xarray<double> log_r = xt::log(xt::where(input > 0, input, 1));
  ep(log)

  bp(multi)
  xt::xarray<double> r2logr = pow_r*log_r;
  ep(multi)

  return r2logr;
}

xt::xarray<double>
compute_weights(xt::xarray<double> X, xt::xarray<double>fX)
{
  bp(cdist)
  auto pairdist = cdist(X,X);
  ep(cdist)

  bp(thin_plate)
  auto A = thin_plate(pairdist);
  ep(thin_plate)

  bp(minus)
  auto lambda = 0.1*xt::eye(A.shape(1));
  A -= lambda;
  ep(minus)
  
  bp(solve)
  auto weights = xt::linalg::solve(A,fX);
  ep(solve)

  return weights;
}

double 
GetValue(xt::xarray<double> point,xt::xarray<double> X, xt::xarray<double> w)
{
  auto pairdist = cdist(point, X);
  auto A = thin_plate(pairdist);

  // std::cout << xt::adapt(A.shape()) << xt::adapt(w.shape()) << std::endl;

  auto res = xt::linalg::dot(A, w)();
  return res;
}
xt::xarray<double> 
load_data(const char* field)
{
  std::string fname(field);
  std::ifstream in_file;
  in_file.open(fname);
  xt::xarray<double> data =xt::load_csv<double>(in_file);
  in_file.close();
  std::cout << xt::adapt(data.shape()) << std::endl;
  return data;
}
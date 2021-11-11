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

namespace xt
{


xt::xarray<double> 
cdist(const xt::xarray<double>& A, const xt::xarray<double>& B)
{

  xarray<double> A_sq =  xt::norm_sq(A,{0});

  xarray<double> B_sq =  xt::norm_sq(B,{0});

  xarray<double> AdotB = xt::linalg::dot(xt::transpose(A),B);

  A_sq.reshape({A.shape(1),1});
  B_sq.reshape({1,B.shape(1)});

  xarray<double> dist = xt::sqrt(A_sq -2*AdotB + B_sq);

  return dist;
}

xt::xarray<double> 
thin_plate(xt::xarray<double>& input)
{

  xt::xarray<double> pow_r = xt::pow(input,2);

  xt::xarray<double> log_r = xt::log(xt::where(input > 0, input, 1));

  xt::xarray<double> r2logr = pow_r*log_r;

  return r2logr;
}
void Rbf::SetData(const xt::xarray<double> &X, const xt::xarray<double> &y) {
    // CHECK_EQ(y.rows(), X.cols());
    this->X = X;
    this->y = y;
}

void Rbf::ComputeWeights(double _lambda) {

  auto pairdist = cdist(X,X);

  auto A = thin_plate(pairdist);

  auto lambda = _lambda*xt::eye(A.shape(1));
  A -= lambda;

  auto weights = xt::linalg::solve(A,this->y);

  this->w = weights;
}

// 2xN

double Rbf::GetValue(const xt::xarray<double> &x) const
{
  auto pairdist = cdist(x, this->X);
  auto A = thin_plate(pairdist);
  double res = xt::linalg::dot(A, this->w)();
  return res;
}

double
Rbf::operator()(std::initializer_list<double> input_list)
{
  xt::xarray<double> x(input_list);
  x.reshape({input_list.size(),1});

  return this->GetValue(x);
}
xt::xarray<double> Rbf::GetValues(const xt::xarray<double> &x) const
{
  auto pairdist = cdist(x, this->X);
  auto A = thin_plate(pairdist);
  auto res = xt::linalg::dot(A, this->w);
  return res;
}

}
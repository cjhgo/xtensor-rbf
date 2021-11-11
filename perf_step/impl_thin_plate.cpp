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

xt::xarray<double> thin_plate(xt::xarray<double>& input)
{

  auto pow_r = xt::pow(input,2);
  auto log_r = xt::log(input);
  xt::xarray<double> res = pow_r*log_r;
  return res;
}

int main(int argc, char* argv[])
{
  xt::xarray<double> a = {
  {1,2,3},
  {3,1,5}};
  xt::xarray<double> b = xt::log(a);
  xt::xarray<double> c = a*b;
  std::cout<<b<<std::endl;
  std::cout<<c<<std::endl;

  xt::xarray<double> d = {
    {1,2,1},
    {1,1,1}
  };
  auto e = a*d;
  std::cout<< e<< std::endl<<std::endl;
  std::cout << thin_plate(a) << std::endl;
  return 0;
}
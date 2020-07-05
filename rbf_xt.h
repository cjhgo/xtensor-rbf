#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <cstdio>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xnorm.hpp>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xmanipulation.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xcsv.hpp>


#define bp(s) auto  begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s) std::cout << "Profile:  "  << #s << " spend time(s): " \
             << (std::chrono::high_resolution_clock::now() - begin_##s)*1.0 / std::chrono::seconds(1) \
             << std::endl << std::endl; 

using xt::xarray;


xt::xarray<double> 
cdist(const xt::xarray<double>& A, const xt::xarray<double>& B);

xt::xarray<double> 
thin_plate(xt::xarray<double>& input);

xt::xarray<double>
compute_weights(xt::xarray<double> X, xt::xarray<double>fX);

double 
GetValue(xt::xarray<double> point,xt::xarray<double> X, xt::xarray<double> w);

xt::xarray<double> 
load_data(const char* field);
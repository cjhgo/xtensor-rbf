#include "algo/rbf.h"
#include "algo/utils.h"

#include <chrono>
#include <cmath>
#include <eigen3/Eigen/LU>
#include <glog/logging.h>
#include <iostream>
#include <string>

using Eigen::MatrixXd;
using Eigen::VectorXd;

extern inline MatType cdist(const MatType &A, const MatType &B);

extern inline MatType thin_plate(MatType &A);


// void Rbf::SetData(const MatrixXf &X, const VectorXf &y) {
//     // CHECK_EQ(y.rows(), X.cols());
//   #if FLOATRBF
//     this->X_ = X;
//     this->y_ = y;
//   #endif
// }

void Rbf::SetData(const MatrixXd &X, const VectorXd &y) {
  #if FLOATRBF
    std::cout << "i am in float" << std::endl;
    this->X_ = X.cast<float>();
    this->y_ = y.cast<float>();
  #else 
    std::cout << "i am in double" << std::endl;
    this->X_ = X;
    this->y_ = y;
  #endif
}

/**
void checknan(MatrixXd a, const char *name, MatrixXd &s) {
  for (int i = 0; i < a.rows(); i++) {
    for (int j = 0; j < a.cols(); j++) {
      if (std::isnan(a(i, j)) || std::isinf(a(i, j))) {
        std::cout << "find nan in " << name << " at " << i << "\t" << j << "\t"
                  << s(i, j) << "\t" << std::log(s(i, j)) << std::endl;
      }
    }
  }
}
**/

void Rbf::ComputeWeights(bool use_regularization, double lambda) {
  profiler_start(ComputeWeights);
  int dim = this->y_.rows();

  auto pairdist = cdist(this->X_, this->X_);

  auto A = thin_plate(pairdist);
  A -= (MatType::Identity(dim, dim) * lambda);

  profiler_start(solve);
  this->w_ = A.lu().solve(this->y_);
  profiler_end(solve);

  profiler_end(ComputeWeights);
}

// 2xN
Eigen::MatrixXd Rbf::GetValues(const Eigen::MatrixXd &input) const {
  #if FLOATRBF
    auto pairdist = cdist(input.cast<float>(), this->X_);
  #else
    auto pairdist = cdist(input, this->X_);
  #endif

  auto A = thin_plate(pairdist);

  auto res = A * this->w_;

  #if FLOATRBF
    return res.cast<double>();
  #else
    return res;
  #endif
}

// MatrixXf Rbf::operator()(const MatrixXf & input) const{
//   auto pairdist = cdist(input, this->X_);

//   auto A = thin_plate(pairdist);

//   auto res = A * this->w_;

//   return res;
// }


double Rbf::GetValue(const VectorXd &x) const {
  #if FLOATRBF
    Eigen::MatrixXf in(2, 1);
    in.col(0) = x.cast<float>();
  #else
    Eigen::MatrixXd in(2, 1);
    in.col(0) = x;
  #endif

  auto pairdist = cdist(in, this->X_);
  auto A = thin_plate(pairdist);
  auto res = A * this->w_;
  return res(0, 0);
}

// float Rbf::operator()(float xf, float yf) const{
//   MatrixXf in(2, 1);
//   in << xf, yf;
//   auto res = (*this)(in);
//   return res(0, 0);
// }

inline MatType cdist(const MatType &meA, const MatType &meB) {
  profiler_start(cdist);
  MatType D = ((-2 * meA.transpose() * meB).colwise() +
                meA.colwise().squaredNorm().transpose())
                   .rowwise() +
               meB.colwise().squaredNorm();

  D = (D.array() < 0).select(0, D);
  profiler_end(cdist);
  return D;
}
inline MatType thin_plate(MatType &A) {
  /**
   * A: r^2, log(r) = log(A)/2
   * thin_plate: r^2log(r)
   **/
  profiler_start(thin_plate);

  profiler_start(log);
  MatType logr = 0.5 * A.array().log();
  profiler_end(log);

  profiler_start(select);
  logr = (logr.array().isInf() == 1).select(0, logr);
  profiler_end(select);

  profiler_start(mutli);
  MatType res = A.array() * logr.array();
  profiler_end(mutli);

  profiler_end(thin_plate);

  return res;
}

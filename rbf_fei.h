#pragma once

#include <ctime>
#include <time.h>
#include <vector>

#include <Eigen/Core>


#include <chrono>
#include <cmath>
#include <Eigen/LU>
#include <iostream>
#include <string>


#define profiler_start(s) auto  begin_##s = std::chrono::high_resolution_clock::now();
#define profiler_end(s) std::cout << "Profile:  "  << #s << " spend time(s): " \
             << (std::chrono::high_resolution_clock::now() - begin_##s)*1.0 / std::chrono::seconds(1) \
             << std::endl << std::endl; 

// #define profiler_start(s) {}
// #define profiler_end(s) {}

class Rbf {
public:
  Rbf()=default;

  // API
  void SetData(const Eigen::MatrixXd &X, const Eigen::VectorXd &y);
  void SetData(const Eigen::MatrixXf &X, const Eigen::VectorXf &y);

  void ComputeWeights(bool use_regularization = false, double lambda = 0.1);

  double GetValue(const Eigen::VectorXd & xd) const;
  Eigen::MatrixXd GetValues(const Eigen::MatrixXd & Xd) const;

  float operator()(float xf, float yf) const;
  Eigen::MatrixXf operator()(const Eigen::MatrixXf& Xf) const;

private:
  // Data points
  Eigen::MatrixXf X_;
  Eigen::VectorXf y_;

  // Weights
  Eigen::VectorXf w_;
  Eigen::VectorXd w2;

};


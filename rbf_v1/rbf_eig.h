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

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

using Eigen::InnerStride;
using Eigen::Map;
using Eigen::MatrixX2d;
using Eigen::MatrixX3d;
using Eigen::MatrixXd;
using Eigen::RowVector3d;
using Eigen::RowVectorXd;
using Eigen::RowVectorXi;
using Eigen::Vector2d;
using Eigen::Vector3d;
using Eigen::VectorXd;

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
  void ComputeWeights(bool use_regularization = false, double lambda = 0.1);
  double GetValue(const Eigen::VectorXd &x) const;
  Eigen::MatrixXd GetValues(const Eigen::MatrixXd &x) const;

  // Getter methods
  const Eigen::VectorXd &GetY() const { return y; }
  const Eigen::MatrixXd &GetX() const { return X; }
  const Eigen::VectorXd &GetW() const { return w; }

private:
  // Data points
  Eigen::MatrixXd X;
  Eigen::VectorXd y;

  // Weights
  Eigen::VectorXd w;

};


#pragma once

#include <ctime>
#include <time.h>
#include <vector>

#include <eigen3/Eigen/Core>

#define FLOATRBF 1

#if FLOATRBF
typedef Eigen::MatrixXf MatType ;
typedef Eigen::VectorXf VecType ;
#else
typedef Eigen::MatrixXd MatType ;
typedef Eigen::VectorXd VecType ;
#endif

class Rbf {
public:
  Rbf() = default;

  // API
  void SetData(const Eigen::MatrixXd &X, const Eigen::VectorXd &y);
  void ComputeWeights(bool use_regularization = false, double lambda = 0.1);
  double GetValue(const Eigen::VectorXd &x) const;
  Eigen::MatrixXd GetValues(const Eigen::MatrixXd &x) const;

  // void SetData(const Eigen::MatrixXf &X, const Eigen::VectorXf &y);
  // float operator()(float xf, float yf) const;
  // Eigen::MatrixXf operator()(const Eigen::MatrixXf& Xf) const;

  // Getter methods
  // const Eigen::VectorXd &GetY() const { return y; }
  // const Eigen::MatrixXd &GetX() const { return X; }
  // const Eigen::VectorXd &GetW() const { return w; }

private:
  // Data points
  MatType X_;
  VecType y_;

  // Weights
  VecType w_;
};

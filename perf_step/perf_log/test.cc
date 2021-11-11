#include <arm_neon.h>
#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "acl_double.h"

using Eigen::VectorXd;

int main(int argc, char const *argv[])
{
  double a = 3.3;
  double a2[2] = {3.3, 2.2};
  std::cout << "std " << 0.5*3.3*std::log(a) << std::endl;
  std::cout << "std " << 0.5*2.2*std::log(2.2) << std::endl;
  float64x2_t armin = vld1q_f64(a2);
  std::cout << armin[0] << " " << armin[1] << std::endl;
  VectorXd d = VectorXd::Random(10);
  std::cout << d << std::endl;
  
  return 0;
}

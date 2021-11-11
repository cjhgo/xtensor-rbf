#include <arm_neon.h>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <chrono>
#include <eigen3/Eigen/Core>
#include "acl_double.h"


using Eigen::VectorXd;

void vary(){

  std::vector<int> size = {8, 10, 12, 16, 18, 50, 100, 200, 500, 1000, 2000, 5000, 10000,1000000};
  for(auto len : size){
    VectorXd  d = VectorXd::Random(len);
    d = (d.array() < 0).select(1, d);
    VectorXd d2 = d;
    bp(aclvary)
    neonlog(d2);
    ep(aclvary)

    bp(eiglog)
    VectorXd d3 = 0.5*d.array()*d.array().log();
    double dsum = d3.sum();
    ep(eiglog)

    std::cout << "sum is " << d2.sum() << " "  << dsum << std::endl;
  }
};

int main(int argc, char const *argv[])
{
  // vary();
  double a = 3.3;
  double a2[2] = {3.3, 0};
  std::cout << "std " << 0.5*3.3*std::log(a) << std::endl;
  std::cout << "std " << 0.5*0*std::log(0) << std::endl;
  float64x2_t armin = vld1q_f64(a2);
  float64x2_t res = vlogq_f64(armin);
  double x = res[1];
  double y = std::log(0);
  std::cout << x << " " << y << std::endl;
  std::cout << 0*x << " " << 0*y << std::endl;
  std::cout << "acl " << res[0] << std::endl;
  std::cout << "acl " << res[1] << std::endl;

  // vary();
  return 0;
}


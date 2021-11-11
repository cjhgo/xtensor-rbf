#include <arm_neon.h>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <chrono>
#include <eigen3/Eigen/Core>
#include "acl_log.h"


using Eigen::VectorXf;

void vary(){

  std::vector<int> size = {8, 10, 12, 16, 18, 50, 100, 200, 500, 1000, 2000, 5000, 10000,1000000};
  for(auto len : size){
    VectorXf  d = VectorXf::Random(len);
    d = (d.array() < 0).select(1, d);
    VectorXf d2 = d;
    bp(aclvary)
    neonlog(d2);
    ep(aclvary)

    bp(eiglog)
    float dsum = d.array().log().sum();
    ep(eiglog)

    std::cout << "sum is " << d2.sum() << " "  << dsum << std::endl;
    // if(len > 10000){
    //   VectorXf me = d.array().log();
    //   for(int i = 0; i < len; i++){
    //     float a = d2(i, 0);
    //     float b = me(i, 0);
    //     if( std::abs( a-b) > 0){
    //       std::cout << " en ? " << i << " " << a << " " << b <<  " " << a-b << std::endl;
    //     }

    //   }
    // }

  }
};

int main(int argc, char const *argv[])
{
  // vary();

  std::vector<float> in = {3.3, 2.2, 3.3, 2.2};
  float32x4_t armin = vld1q_f32(in.data());
  float32x4_t res = vlogq_f32(armin);
  return 0;
}


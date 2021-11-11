#include <arm_neon.h>
#include <iostream>
#include <cmath>


int main(int argc, char const *argv[])
{
  double a2[2] = {3.3, 2.2};
  double a3[2] = {0.5, 1.8};
  float64x2_t a2neon = vld1q_f64(a2);
  float64x2_t a3neon = vld1q_f64(a3);
  float64x2_t resneon = a2neon + a3neon;
  std::cout << resneon[0] << " " << resneon[1] << std::endl;
  double res[2];
  vst1q_f64(res, resneon);

  return 0;
}

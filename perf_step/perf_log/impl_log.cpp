#include <arm_neon.h>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <chrono>
#include <eigen3/Eigen/Core>
#include "acl_log.h"


int main(int argc, char const *argv[])
{
  std::vector<float> in = {0.1, 0.2, 0.3, 0.4};
  float32x4_t armin = vld1q_f32(in.data());

  bp(armlog)
  float32x4_t res = vlogq_f32(armin);
  float out[4];
  vst1q_f32(out, res);
  ep(armlog)

  std::cout << out[0] << std::endl;
  std::cout << out[1] << std::endl;
  std::cout << out[2] << std::endl;
  std::cout << out[3] << std::endl;
  float out2[4];// = {0,0,0,0};
  bp(stdlog)
  out2[0]=std::log(in[0]);
  out2[1]=std::log(in[1]);
  out2[2]=std::log(in[2]);
  out2[3]=std::log(in[3]);
  ep(stdlog)
  std::cout << out2[0] << std::endl;
  std::cout << out2[1] << std::endl;
  std::cout << out2[2] << std::endl;
  std::cout << out2[3] << std::endl;

  Eigen::MatrixXf a = Eigen::MatrixXf::Random(1000, 1000);
  a = (a.array() < 0).select(99, a);
  Eigen::MatrixXf c = Eigen::MatrixXf::Zero(1000, 1000);

  Eigen::MatrixXf b = a.array().log();
  std::cout << " a(0,0) is " << a(0,0) << std::endl
            << " log(a(0,0)) is " << std::log(a(0,0)) << std::endl
            << " b(0,0) is " << b(0,0) << std::endl
            << "a size is " << a.size() << std::endl;
  float32x4_t a04 = vld1q_f32(a.data());
  float32x4_t a04log = vlogq_f32(a04);
  vst1q_f32(c.data(), a04log);
  std::cout << a04[0] << " " 
            << a04[1] << " "
            << a04[2] << " "
            << a04[3] << std::endl
            << a(0,0) << " "
            << a(1,0) << " "
            << a(2,0) << " "
            << a(3,0) << std::endl
            << b(0,0) << " "
            << b(1,0) << " "
            << b(2,0) << " "
            << b(3,0) << std::endl
            << c(0,0) << " "
            << c(1,0) << " "
            << c(2,0) << " "
            << c(3,0) << std::endl;

  Eigen::MatrixXf ori = Eigen::MatrixXf::Random(1000, 1000);
  Eigen::MatrixXf fl = Eigen::MatrixXf::Zero(1000, 1000);
  ori = (ori.array() < 0).select(2, ori);
  Eigen::MatrixXf gg = fl * 0.5;
  fl = ori;
  bp(eigenlog)
  Eigen::MatrixXf orilog = ori.array().log();
  ep(eigenlog)

  bp(neonlog)
  neonlog(ori);
  ep(neonlog)

  bp(fl)
  for(int i = 0; i < 1000*1000; i++){
    fl.data()[i] = std::log(fl.data()[i]);
  }
  ep(fl)
  std::cout << orilog(4,0) << " "
            << orilog(5,0) << " "
            << orilog(6,0) << " "
            << orilog(8,0) << std::endl
            << ori(4,0) << " "
            << ori(5,0) << " "
            << ori(6,0) << " "
            << ori(8,0) << std::endl
            << orilog.sum() << std::endl
            << fl.sum() << std::endl
            << ori.sum() << std::endl;

  return 0;
}


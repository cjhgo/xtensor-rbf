#include <arm_neon.h>
#include <cmath>
#include <iostream>

/**
 * int32x4_t vdupq_n_s32 (int32_t value) 
 * uint32x4_t vreinterpretq_u32_f32 (float32x4_t a)
 * 
 **/


int main(int argc, char const *argv[])
{
  // vary();
  float a2[4] = {8.75, 8.75, 8.75, 8.75};
  float32x4_t a = vld1q_f32(a2);
  uint32x4_t b = vreinterpretq_u32_f32(a);
  float c = 8.75;
  uint d =  1091305472;

  std::cout << b[0] << " " << b[1] << " " << b[2] << " " << b[3] << std::endl;
  std::cout << 0*0*std::log(0) << std::endl;
  float nan = 0*std::log(0);

  std::cout << (nan&&0x00000000) <<std::endl;
  float num = 3.3*std::log(3.3);
  typedef union {
    uint32_t i;
    float f;
  } IorF_T;
  IorF_T u;
  u.f = 3.3*std::log(3.3);
  float f = 3.3*std::log(3.3);
  f = f==f? f :0;
  float f2 = 0*std::log(0);
  f2 = f2==f2? f2 :0;
  std::cout << f << " " << f2 << std::endl;

  float fnan[4] = {3.3, nan,  0, 5.1};
  float32x4_t fin = vld1q_f32(fnan);
  std::cout << fin[1] << std::endl;
  uint32x4_t vmask = vceqq_f32(fin, fin);
  fin = (float32x4_t)vandq_u32((uint32x4_t)fin, vmask);
  std::cout << fin[0] << " " << fin[1] << " " << fin[2] <<  " " << fin[3] << std::endl;

  float infv = -std::log(0);
  std::cout << "get " << infv << std::endl;

  float what = infv * std::log(infv);
  std::cout << "get " << what << std::endl;





  // std::cout << (float)((uint)num&&0xffffffff) <<std::endl;
  // typedef union {
  //   uint32_t i;
  //   float f;
  // } U;

  // U u;
  // u.f = 3.3*std::log(3.3);

  // uint32_t me = u.i & 0xffffffff;
  // uint32_t l = u.i;
  // uint32_t r = 0xffffffff;
  // uint32_t res = l & r;
  // std::cout << u.f << std::endl;
  // std::cout << u.i << std::endl;
  // std::cout << num << std::endl;
  // std::cout << res << std::endl;
  return 0;
}


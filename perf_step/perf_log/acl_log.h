#include <arm_neon.h>

#define bp(s) auto begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s)                                                                  \
  std::cout << "Profile: " << #s << " spend time : \t"                         \
            << std::chrono::duration<double, std::milli>(                      \
                   std::chrono::high_resolution_clock::now() - begin_##s)      \
                   .count()                                                    \
            << std::endl                                                       \
            << std::endl;

static  std::array<float32x4_t, 8> coeffs = // take out...
{
    {
        vdupq_n_f32(-2.29561495781f),
        vdupq_n_f32(-2.47071170807f),
        vdupq_n_f32(-5.68692588806f),
        vdupq_n_f32(-0.165253549814f),
        vdupq_n_f32(5.17591238022f),
        vdupq_n_f32(0.844007015228f),
        vdupq_n_f32(4.58445882797f),
        vdupq_n_f32(0.0141278216615f),
    }
};

inline float32x4_t vtaylor_polyq_f32(float32x4_t x)
{
  float32x4_t A   = vmlaq_f32(coeffs[0], coeffs[4], x);
  float32x4_t B   = vmlaq_f32(coeffs[2], coeffs[6], x);
  float32x4_t C   = vmlaq_f32(coeffs[1], coeffs[5], x);
  float32x4_t D   = vmlaq_f32(coeffs[3], coeffs[7], x);
  float32x4_t x2  = vmulq_f32(x, x);
  float32x4_t x4  = vmulq_f32(x2, x2);
  float32x4_t res = vmlaq_f32(vmlaq_f32(A, B, x2), vmlaq_f32(C, D, x2), x4);
  return res;
}


inline float32x4_t vlogq_f32(float32x4_t x)
{
    static const int32x4_t   CONST_127 = vdupq_n_s32(127);           // 127
    static const float32x4_t CONST_LN2 = vdupq_n_f32(0.6931471805f); // ln(2)

    // Extract exponent
    int32x4_t   m   = vsubq_s32(vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_f32(x), 23)), CONST_127);

    std::cout << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << std::endl;

    float32x4_t val = vreinterpretq_f32_s32(vsubq_s32(vreinterpretq_s32_f32(x), vshlq_n_s32(m, 23)));

    // Polynomial Approximation
    float32x4_t poly = vtaylor_polyq_f32(val);

    // Reconstruct
    poly = vmlaq_f32(poly, vcvtq_f32_s32(m), CONST_LN2);

    return poly;
}

void neonlog(Eigen::MatrixXf& ori){
  int multiple = ori.size()/4;
  for(int i = 0; i < multiple; i++){
    float * data = ori.data() + i*4;
    float32x4_t a04 = vld1q_f32(data);
    float32x4_t a04log = vlogq_f32(a04);
    vst1q_f32(data, a04log);
  }
  for(int i = 4 * multiple; i < ori.size(); i++){
    ori.data()[i] = std::log(ori.data()[i]);
  }
}

void neonlog(Eigen::VectorXf& ori){
  int multiple = ori.size()/4;
  float * begin = ori.data();
  for(int i = 0; i < multiple; i++){
    float * data = begin + i*4;
    float32x4_t a04 = vld1q_f32(data);
    float32x4_t a04log = vlogq_f32(a04);
    vst1q_f32(data, a04log);
  }
  for(int i = 4 * multiple; i < ori.size(); i++){
    ori.data()[i] = std::log(ori.data()[i]);
  }
}
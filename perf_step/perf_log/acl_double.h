#include <arm_neon.h>

#define bp(s) auto begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s)                                                                  \
  std::cout << "Profile: " << #s << " spend time : \t"                         \
            << std::chrono::duration<double, std::milli>(                      \
                   std::chrono::high_resolution_clock::now() - begin_##s)      \
                   .count()                                                    \
            << std::endl                                                       \
            << std::endl;

static  std::array<float64x2_t, 8> coeffs = // take out...
{
    {
        vdupq_n_f64(-2.29561495781f),
        vdupq_n_f64(-2.47071170807f),
        vdupq_n_f64(-5.68692588806f),
        vdupq_n_f64(-0.165253549814f),
        vdupq_n_f64(5.17591238022f),
        vdupq_n_f64(0.844007015228f),
        vdupq_n_f64(4.58445882797f),
        vdupq_n_f64(0.0141278216615f),
    }
};

inline float64x2_t vtaylor_polyq_f64(float64x2_t x)
{
  float64x2_t A   = vmlaq_f64(coeffs[0], coeffs[4], x);
  float64x2_t B   = vmlaq_f64(coeffs[2], coeffs[6], x);
  float64x2_t C   = vmlaq_f64(coeffs[1], coeffs[5], x);
  float64x2_t D   = vmlaq_f64(coeffs[3], coeffs[7], x);
  float64x2_t x2  = vmulq_f64(x, x);
  float64x2_t x4  = vmulq_f64(x2, x2);
  float64x2_t res = vmlaq_f64(vmlaq_f64(A, B, x2), vmlaq_f64(C, D, x2), x4);
  return res;
}


inline float64x2_t vlogq_f64(float64x2_t x)
{
    static const int64x2_t   CONST_127 = vdupq_n_s64(1023);           // 127
    static const float64x2_t CONST_LN2 = vdupq_n_f64(0.6931471805f); // ln(2)
    static const float64x2_t CONST_HALF = vdupq_n_f64(0.5); // ln(2)
    float64x2_t half = CONST_HALF * x;

    // std::cout << x[0] << " " << x[1] << std::endl;
    // std::cout << CONST_127[0] << " " << CONST_127[1] << std::endl;
    // std::cout << CONST_LN2[0] << " " << CONST_LN2[1] << std::endl;

    // Extract exponent
    int64x2_t   m   = vsubq_s64(
      vreinterpretq_s64_u64(
        vshrq_n_u64(
          vreinterpretq_u64_f64(x), 52
          )
      ), 
      CONST_127);

    // std::cout << m[0] << " " << m[1] << std::endl;

    float64x2_t val = vreinterpretq_f64_s64(vsubq_s64(vreinterpretq_s64_f64(x), vshlq_n_s64(m, 52)));

    // Polynomial Approximation
    float64x2_t poly = vtaylor_polyq_f64(val);

    // Reconstruct
    poly = vmlaq_f64(poly, vcvtq_f64_s64(m), CONST_LN2);

    return poly*half;
}

void neonlog(Eigen::MatrixXd& ori){
  int multiple = ori.size()/4;
  for(int i = 0; i < multiple; i++){
    double * data = ori.data() + i*4;
    float64x2_t a04 = vld1q_f64(data);
    float64x2_t a04log = vlogq_f64(a04);
    vst1q_f64(data, a04log);
  }
  for(int i = 4 * multiple; i < ori.size(); i++){
    ori.data()[i] = std::log(ori.data()[i]);
  }
}

void neonlog(Eigen::VectorXd& ori){
  int multiple = ori.size()/2;
  double * begin = ori.data();
  for(int i = 0; i < multiple; i++){
    double * data = begin + i*2;
    float64x2_t a04 = vld1q_f64(data);
    float64x2_t a04log = vlogq_f64(a04);
    vst1q_f64(data, a04log);
  }
  for(int i = 2 * multiple; i < ori.size(); i++){
    ori.data()[i] = 0.5*(ori.data()[i])*std::log(ori.data()[i]);
  }
}
#include "rbf_xt.h"

int main(int argc, char* argv[])
{
  auto x = load_data("./data/py-input-x");
  auto y = load_data("./data/py-input-y");
  auto z = load_data("./data/py-input-z");
  auto w = load_data("./data/py-get-w");

  auto xy = xt::transpose(xt::hstack(xt::xtuple(x, y)));
  bp(cw)
  auto ww = compute_weights(xy,z);
  ep(cw)
  // (788.826836126515, 320.0, 988.0)


  /**
  xt::xarray<double> point {788.826836126515,320};
  point.reshape({2,1});
  bp(gv)
  auto pred = GetValue(point, xy, ww);
  ep(gv)
  std::cout << pred << std::endl;
  **/
  return 0;
}
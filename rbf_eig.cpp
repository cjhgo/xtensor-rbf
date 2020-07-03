#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <cstdio>
#include <vector>
#include <string>
#include "rbf.h"


RowVectorXd
load_data(const char* field)
{
  std::vector<double> data;
  std::ifstream in_file;
  std::string fname(field);
  in_file.open(fname);
  double tmp;
  while (in_file>>tmp)
  {
    data.push_back(tmp);
  }
  std::cout << data.size() << std::endl;
  int number_of_samples = data.size();
  auto res = Map<RowVectorXd>(data.data(), data.size());
  return res;
}



int main(int argc, char* argv[])
{
  auto row_0_x = load_data("./data/py-input-x");
  auto row_1_y = load_data("./data/py-input-y");
  auto z = load_data("./data/py-input-z").transpose();
  auto w = load_data("./data/py-get-w");

  int N = row_0_x.cols();
  MatrixXd input(2, N);
  input.row(0) = row_0_x;
  input.row(1) = row_1_y;

  std::cout << input.rows() << "\t" << input.cols() << std::endl
            << z.rows()<< "\t" << z.cols() << std::endl;

  Rbf rbf;
  rbf.SetData(input,z);
  profiler_start(compute)
  rbf.ComputeWeights();
  profiler_end(compute)

  return 0;
}
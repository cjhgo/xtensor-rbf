#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <cstdio>
#include <vector>
#include <string>
#include "rbf_fei.h"


using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

using Eigen::InnerStride;
using Eigen::Map;
using Eigen::MatrixX2d;
using Eigen::MatrixX3d;
using Eigen::MatrixXd;
using Eigen::RowVector3d;
using Eigen::RowVectorXd;
using Eigen::RowVectorXi;
using Eigen::Vector2d;
using Eigen::Vector3d;
using Eigen::VectorXd;

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
  VectorXd z = load_data("./data/py-input-z").transpose();
  auto w = load_data("./data/py-get-w");
  
  std::cout << "after file " << z(0,0) << std::endl;

  int N = row_0_x.cols();
  MatrixXd input(2, N);
  input.row(0) = row_0_x;
  input.row(1) = row_1_y;

  std::cout << input.rows() << "\t" << input.cols() << std::endl
            << z.rows()<< "\t" << z.cols() << std::endl;

  Rbf rbf;
  rbf.SetData(input,z);
  rbf.ComputeWeights();

  std::cout << " at 0 0 " << rbf(0, 0) << std::endl;
  VectorXd in(2);
  in << input(0,0), input(1, 0);


  std::cout << " at 0 0 " << rbf.GetValue(in) << std::endl;

  return 0;
}
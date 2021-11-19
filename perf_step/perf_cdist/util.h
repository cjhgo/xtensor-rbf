#pragma once

#include <cstdio>
#include <chrono>
#include <Eigen/Eigen>
#include <iostream>

#define bp(s) auto  begin_##s = std::chrono::high_resolution_clock::now();
#define ep(s) std::cout << "Profile:  "  << #s << " spend time(s): " \
             << (std::chrono::high_resolution_clock::now() - begin_##s)*1.0 / std::chrono::seconds(1) \
             << std::endl << std::endl; 


using Eigen::Matrix;
using Eigen::MatrixXd;


void print_matrix(int m, int n, double * A, int lda);

double compare_matrices(int m, int n, double * A, int lda, double * B, int ldb);


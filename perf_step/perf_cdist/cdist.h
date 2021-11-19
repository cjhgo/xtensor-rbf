/**
 * @file cdist.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 * C(m,n)=A(m,k)^B(k,n)
 * A 是 m 个 k 维 列向量, 列优先存储
 * B 是 n 个 k 维 列向量, 列优先存储
 * 
 * 
 */

#pragma once
#include "util.h"


void cdist_eigen_bin_decom(MatrixXd& A, MatrixXd& B, MatrixXd& C);

void cdist_eigen_vwise(MatrixXd& A, MatrixXd& B, MatrixXd& C);


void cdist_forloop_mkn(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_mnk(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);


void cdist_forloop_nmk(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_nkm(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_kmn(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

void cdist_forloop_knm(int m, int n, int k, double * A, int lda,
																						double * B, int ldb,
																						double * C, int ldc);

inline double l2_dist(double a, double b){
	return (a-b) * (a-b);
}

void cdist_gemm_block(int m, int n, int k, double * A, int lda,
																					 double * B, int ldb,
																					 double * C, int ldc);


void cdist_gemm_BU(int m, int n, int k, double * A, int lda,
																				double * B, int ldb,
																				double * C, int ldc);

void cdist_gemm_BUP(int m, int n, int k, double * A, int lda,
																				double * B, int ldb,
																				double * C, int ldc);

void cdist_gemm_BUPN(int m, int n, int k, double * A, int lda,
																					double * B, int ldb,
																					double * C, int ldc);
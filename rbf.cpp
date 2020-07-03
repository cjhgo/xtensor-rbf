#include "rbf.h"

extern inline VectorXd SolveLinearSystem(const MatrixXd &A, const VectorXd &y);

extern inline MatrixXd cdist(const MatrixXd &A, const MatrixXd &B);

extern inline MatrixXd thin_plate(MatrixXd &A);

void Rbf::SetData(const MatrixXd &X, const VectorXd &y) {
    // CHECK_EQ(y.rows(), X.cols());
    this->X = X;
    this->y = y;
}

void checknan(MatrixXd a, const char *name, MatrixXd &s) {
    for (int i = 0; i < a.rows(); i++) {
	for (int j = 0; j < a.cols(); j++) {
	    if (std::isnan(a(i, j)) || std::isinf(a(i, j))) {
		std::cout << "find nan in " << name << " at " << i << "\t" << j << "\t"
		    << s(i, j) << "\t" << std::log(s(i, j)) << std::endl;
	    }
	}
    }
}
void Rbf::ComputeWeights(bool use_regularization, double lambda) {
    profiler_start(ComputeWeights);
    int dim = y.rows();

    auto pairdist = cdist(this->X, this->X);

    auto A = thin_plate(pairdist);

    profiler_start(minus)
    A -= (MatrixXd::Identity(dim, dim) * lambda);
    profiler_end(minus)

    profiler_start(solve);
    w = A.lu().solve(this->y);
    profiler_end(solve);

    profiler_end(ComputeWeights);
}

// 2xN
MatrixXd Rbf::GetValues(const MatrixXd &input) const {

    auto pairdist = cdist(input, this->X);

    auto A = thin_plate(pairdist);

    auto res = A * this->w;

    return res;
}

double Rbf::GetValue(const VectorXd &x) const {

    MatrixXd in(2, 1);
    in.col(0) = x;

    auto pairdist = cdist(in, this->X);

    MatrixXd A = thin_plate(pairdist);

    auto res = A * this->w;

    return res(0, 0);
}

inline VectorXd SolveLinearSystem(const MatrixXd &A, const VectorXd &y) {

    return A.lu().solve(y);
}
inline MatrixXd cdist(const MatrixXd &meA, const MatrixXd &meB) {
    profiler_start(cdist);
    MatrixXd D = ((-2 * meA.transpose() * meB).colwise() +
	    meA.colwise().squaredNorm().transpose())
	.rowwise() +
	meB.colwise().squaredNorm();

    D = (D.array() < 0).select(0, D);
    profiler_end(cdist);
    return D;
}
inline MatrixXd thin_plate(MatrixXd &A) {
    profiler_start(thin_plate);

    profiler_start(pow);
    MatrixXd r = A.array().pow(0.5);
    profiler_end(pow);

    profiler_start(log);
    MatrixXd logr = r.array().log();
    profiler_end(log);

    profiler_start(select);
    logr = (logr.array().isInf() == 1).select(0, logr);
    profiler_end(select);

    profiler_start(mutli);
    MatrixXd res = A.array() * logr.array();
    profiler_end(mutli);

    profiler_end(thin_plate);

    return res;
}

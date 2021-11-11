#include "rbf_fei.h"

using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::MatrixXd;
using Eigen::VectorXd;


extern inline MatrixXf cdist(const MatrixXf &A, const MatrixXf &B);

extern inline MatrixXf thin_plate(MatrixXf &A);


void Rbf::SetData(const MatrixXf &X, const VectorXf &y) {
    // CHECK_EQ(y.rows(), X.cols());
    this->X_ = X;
    this->y_ = y;
}

void Rbf::SetData(const MatrixXd &X, const VectorXd &y) {
    MatrixXf Xf = X.cast<float>();
    VectorXf yf = y.cast<float>();
    this->SetData(Xf, yf);
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
    int dim = this->y_.rows();

    auto pairdist = cdist(this->X_, this->X_);

    auto A = thin_plate(pairdist);

    profiler_start(minus)
    A -= (MatrixXf::Identity(dim, dim) * lambda);
    profiler_end(minus)

    profiler_start(solve);
    this->w_ = A.lu().solve(this->y_);
    profiler_end(solve);

    Eigen::MatrixXd  AA = A.cast<double>();
    Eigen::VectorXd  yy = y_.cast<double>();
    this->w2 = AA.lu().sove(yy);

    profiler_end(ComputeWeights);
}

// 2xN
MatrixXd Rbf::GetValues(const MatrixXd &input) const {
    return (*this)(input.cast<float>()).cast<double>();
}

MatrixXf Rbf::operator()(const MatrixXf & input) const{

    auto pairdist = cdist(input, this->X_);

    auto A = thin_plate(pairdist);

    auto res = A * this->w_;

    return res;
}


double Rbf::GetValue(const VectorXd &x) const {
    float xf = x(0,0);
    float yf = x(1,0);
    std::cout << "en ? " << (*this)(xf, yf) << std::endl;

    std::cout << "at rbf fei : " <<
    this->X_(0, 1) << " " << this->X_(0, 2) << " " << this->X_(0, 3) << std::endl <<
    this->X_(1, 1) << " " << this->X_(1, 2) << " " << this->X_(1, 3) << std::endl <<
    this->y_(0, 0) << " " << this->y_(1, 0) << " " << this->y_(2, 0) << std::endl;

    return (*this)(xf, yf);
}

float Rbf::operator()(float xf, float yf) const{
    MatrixXf in(2, 1);
    in << xf, yf;
    auto res = (*this)(in);
    return res(0, 0);
}

inline MatrixXf cdist(const MatrixXf &meA, const MatrixXf &meB) {
    profiler_start(cdist);
    MatrixXf D = ((-2 * meA.transpose() * meB).colwise() +
	    meA.colwise().squaredNorm().transpose())
	.rowwise() +
	meB.colwise().squaredNorm();

    D = (D.array() < 0).select(0, D);
    profiler_end(cdist);
    return D;
}

inline MatrixXf thin_plate(MatrixXf &A) {
    profiler_start(thin_plate);

    auto ptr = A.data();
    profiler_start(log);
    MatrixXf logr = 0.5 * A.array().log();
    profiler_end(log);

    profiler_start(select);
    logr = (logr.array().isInf() == 1).select(0, logr);
    profiler_end(select);

    profiler_start(mutli);
    MatrixXf res = A.array() * logr.array();
    profiler_end(mutli);

    profiler_end(thin_plate);

    return res;
}

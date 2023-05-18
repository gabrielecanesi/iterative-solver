#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include <iostream>
#include <solver/IterativeSolver.h>
#include <updateStrategy/ConjugateGradientUpdateStrategy.h>
#include <updateStrategy/GradientUpdateStrategy.h>
#include <updateStrategy/GaussSeidelUpdateStrategy.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <util/PowerMethod.h>
#include <limits>
#include <cmath>
#include <memory>

template <typename T, typename MatrixType>
    class IterativeSolver;
namespace MatrixUtil {

    template <typename T>
    void checkSymmetricAndPositiveDefinite(const Eigen::SparseMatrix<T> &A) {
        if (!A.isApprox(A.transpose())) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
        Eigen::SimplicialLLT<Eigen::SparseMatrix<T>> llt(A);
        if (llt.info() == Eigen::NumericalIssue) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
    }

    template <typename T>
    void checkSymmetricAndPositiveDefinite(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A) {
        if (!A.isApprox(A.transpose())) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
        Eigen::LLT<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> llt(A);
        if (llt.info() == Eigen::NumericalIssue) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
    }


    

    template<typename T, typename MatrixType>
    T hagerMethod(MatrixType &A) {

        Eigen::Matrix<T, Eigen::Dynamic, 1> E(A.rows(), 1);
        Eigen::Matrix<T, Eigen::Dynamic, 1> x(A.rows(), 1);

        T op = 1 / (T)A.cols();
        
        for (int i = 0; i < A.cols(); ++i) {
            x(i, 0) = op;
        }

        auto strategy = std::make_shared<UpdateStrategy::ConjugateGradientUpdateStrategy<T, MatrixType>>();
		IterativeSolver<T, MatrixType> solver(20, strategy, 1e-10, true, NormType::EUCLIDEAN, true);
        MatrixType At = A.transpose();
        T yNorm = 0;
        int maxIter = 1000;

        do {
            auto y = solver.solve(A, x)->solution();
            E = y->array().sign();
            auto z = solver.solve(At, E)->solution();

            yNorm = y->template lpNorm<1>();

            if (z->template lpNorm<Eigen::Infinity>() <= z->transpose() * x) {
                return yNorm;
            }

            int maxZr, maxZc;
            z->col(0).maxCoeff(&maxZr, &maxZc);

            x.setZero();
            x(maxZr, 0) = 1;

        } while(maxIter-- > 0);

        return yNorm;
    }

    template<typename T, typename MatrixType> 
	T conditionNumber(MatrixType &A, double tol, unsigned int maxIter){
        T inverseNormEstimate = hagerMethod<T, MatrixType>(A);
        if (std::isnan(inverseNormEstimate)) {
            return std::numeric_limits<T>::infinity();
        }
        return (Eigen::RowVectorXd::Ones(A.rows()) * A.cwiseAbs()).maxCoeff() * hagerMethod<T, MatrixType>(A);
	}

}

#endif
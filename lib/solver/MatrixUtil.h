#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include <iostream>
#include <solver/IterativeSolver.h>
#include <updateStrategy/ConjugateGradientUpdateStrategy.h>
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

        Eigen::Matrix<T, Eigen::Dynamic, 1> b(A.rows(), 1);
        Eigen::Matrix<T, Eigen::Dynamic, 1> y(A.rows(), 1);

        T op = 1 / (T)A.cols();
        
        for (int i = 0; i < A.cols(); ++i) {
            b(i, 0) = op;
        }

        auto strategy = std::make_shared<UpdateStrategy::ConjugateGradientUpdateStrategy<T, MatrixType>>();
		IterativeSolver<T, MatrixType> solver(50, strategy, 1e-2, true, NormType::EUCLIDEAN, true);
        MatrixType At = A.transpose();
        T rho = 0;
        int maxIter = 10;

        do {
            std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> solution = solver.solve(A, b)->solution();
            T xNorm = solution.get()->template lpNorm<1>();
        


            if (xNorm <= rho) {
                return rho;
            }

            rho = xNorm;

            for (int i = 0; i < A.cols(); ++i) {
                if ((*solution.get())(i, 0) < 0) {
                    y(i, 0) = -1;
                } else {
                    y(i, 0) = 1;
                }
            }
            
            auto z = solver.solve(At, y)->solution();
            int maxZ = 0;
            for (int i = 1; i < A.cols(); ++i) {
                if ((*z.get())(i, 0) > maxZ) {
                    maxZ = i;
                }
            }

            if ((*z.get())(maxZ) < z.get()->transpose() * b) {
                return rho;
            }

            for (int i = 0; i < A.cols(); ++i) {
                b(i, 0) = i == maxZ ? 1 : 0;
            }

        } while(maxIter-- > 0);

        return rho;
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
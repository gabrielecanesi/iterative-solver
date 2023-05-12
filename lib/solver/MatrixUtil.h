#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

#include <iostream>
#include <solver/IterativeSolver.h>
#include <updateStrategy/ConjugateGradientUpdateStrategy.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <util/PowerMethod.h>

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

        UpdateStrategy::ConjugateGradientUpdateStrategy<T, MatrixType> strategy;
		IterativeSolver<T, MatrixType> solver(50, &strategy, 1e-2, true, NormType::EUCLIDEAN, true);
        MatrixType At = A.transpose();
        T rho = 0;
        int maxIter = 10;

        do {
            Eigen::Matrix<T, Eigen::Dynamic, 1> solution = *solver.solve(A, b)->solution();
            T xNorm = solution.template lpNorm<1>();
        


            if (xNorm <= rho) {
                return rho;
            }

            rho = xNorm;

            for (int i = 0; i < A.cols(); ++i) {
                if (solution(i, 0) < 0) {
                    y(i, 0) = -1;
                } else {
                    y(i, 0) = 1;
                }
            }
            
            auto z = *solver.solve(At, y)->solution();
            int maxZ = 0;
            for (int i = 1; i < A.cols(); ++i) {
                if (z(i, 0) > maxZ) {
                    maxZ = i;
                }
            }

            if (z(maxZ) < z.transpose() * b) {
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
        return (Eigen::RowVectorXd::Ones(A.rows()) * A.cwiseAbs()).maxCoeff() * hagerMethod<T, MatrixType>(A);
	}

}

#endif
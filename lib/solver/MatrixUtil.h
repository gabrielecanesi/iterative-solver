#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H


#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <util/PowerMethod.h>

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
	T conditionNumber(MatrixType &A, double tol, unsigned int maxIter){
		T maxA = powerMethod<T, MatrixType>(A, tol, maxIter);
		T minA = powerMethodInverse<T, MatrixType>(A, tol, maxIter);

		return abs(maxA)/abs(minA);
	}
}

#endif
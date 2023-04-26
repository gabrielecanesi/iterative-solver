#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H


#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>

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

    template <typename Solver, typename T>
    T computeCondition(Solver &eigs, unsigned int n) {
        eigs.init();
        int nconv = eigs.compute(Spectra::SortRule::LargestMagn);
        double max = eigs.eigenvalues()(0).real();
        double min =  eigs.eigenvalues()(n - 3).real();

        if (min <= ZERO_THRESHOLD) {
            return 1e15;
        }

        return max / min;
    }

    template <typename T>
    T conditionNumber(const Eigen::SparseMatrix<T> &A) {
        Spectra::SparseGenMatProd<T> op(A);
        Spectra::GenEigsSolver<Spectra::SparseGenMatProd<T>> eigs(op, A.cols() - 2, A.cols());
        return computeCondition(eigs, A.cols());
    }

    template <typename T>
    T conditionNumber(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A) {
        Spectra::DenseGenMatProd<T> op(A);
        Spectra::GenEigsSolver<Spectra::DenseGenMatProd<T>> eigs(op, A.cols() - 2, A.cols());
        return computeCondition(eigs, A.cols());
    }
}

#endif
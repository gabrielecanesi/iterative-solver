#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "Solver.h"
#include "updateStrategy/Strategy.h"
#include <Eigen/Cholesky>
#include "exceptions/NonSymmetricAndPositiveDefiniteException.h"
#include "norm.h"
#include "exceptions/NonSquareMatrixException.h"

template<typename T, typename MatrixType>
class IterativeSolver : AbstractSolver<T, MatrixType> {

private:
    unsigned int maxIter;
    unsigned int iterations;
    UpdateStrategy::Strategy<T, MatrixType>* updateStrategy;
    T tol;
    bool skipMatrixCheck = false;
    NormType normType;

    bool reachedTolerance(const Eigen::Matrix<T, Eigen::Dynamic, 1> &currentResult, const MatrixType &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) const {
        double abNorm;
        double bNorm;
        switch (normType) {
            case NormType::EUCLIDEAN:
                abNorm = (A * currentResult- b).norm();
                bNorm = b.norm();
                break;
            case NormType::MANHATTAN:
                abNorm = (A * currentResult - b).template lpNorm<1>();
                bNorm = b.template lpNorm<1>();
                break;
            case NormType::INFTY:
                abNorm = (A * currentResult - b).template lpNorm<Eigen::Infinity>();
                bNorm = b.template lpNorm<Eigen::Infinity>();
                break;
            default:
                break;
        }
        
        return (abNorm / bNorm) < tol;
    }

    void checkSymmetricAndPositiveDefinite(const Eigen::SparseMatrix<T> &A) {
        if (!A.isApprox(A.transpose())) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
        Eigen::SimplicialLLT<Eigen::SparseMatrix<T>> llt(A);
        if (llt.info() == Eigen::NumericalIssue) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
    }

    void checkSymmetricAndPositiveDefinite(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A) {
        if (!A.isApprox(A.transpose())) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
        Eigen::LLT<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> llt(A);
        if (llt.info() == Eigen::NumericalIssue) {
            throw NonSymmetricAndPositiveDefiniteException();
        }
    }

public:
    IterativeSolver(unsigned int maxIter, UpdateStrategy::Strategy<T, MatrixType>* const updateStrategy, T tol, bool skipMatrixCheck, NormType normType = NormType::EUCLIDEAN) : AbstractSolver<T, MatrixType>(),
                                                                                                                                        maxIter(maxIter),
                                                                                                                                        updateStrategy(updateStrategy),
                                                                                                                                        tol(tol),
                                                                                                                                        skipMatrixCheck(skipMatrixCheck),
                                                                                                                                        normType(normType) {}
    IterativeSolver(const IterativeSolver<T, MatrixType> &other) : updateStrategy(nullptr) {
        if (other.updateStrategy != nullptr) {
            this->updateStrategy = other.updateStrategy->clone();
        }
        this->maxIter = other.maxIter;
        this->iterations = other.iterations;
        this->tol = other.tol;
        this->skipMatrixCheck = other.skipMatrixCheck;
    }
    unsigned int neededIterations() const {
        return this->iterations;
    }

    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>

    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
        if (A.rows() != A.cols()) {
            throw NonSquareMatrixException();
        }
        
        if(!skipMatrixCheck) {
            checkSymmetricAndPositiveDefinite(A);
        }

        updateStrategy->init(A, b);
        const Eigen::Matrix<T, Eigen::Dynamic, 1> *currentResult;
        unsigned int iter = 0;

        do {
            currentResult = updateStrategy->update();
            ++iter;
        } while (iter < maxIter && !reachedTolerance(*currentResult, A, b, tol));

        this->iterations = iter;
        return *currentResult;
    }

    std::string methodName() const {
        if (updateStrategy == nullptr) {
            return "";
        }
        return updateStrategy->name();
    }

    T tolerance() const {
        return tol;
    }
};

#endif

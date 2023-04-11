#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "Solver.h"
#include "updateStrategy/Strategy.h"

template<typename T, typename MatrixType>
class IterativeSolver : AbstractSolver<T, MatrixType> {

private:
    unsigned int maxIter;
    unsigned int iterations;
    UpdateStrategy::Strategy<T, MatrixType>* updateStrategy;
    T tol;

    bool reachedTolerance(const Eigen::Matrix<T, Eigen::Dynamic, 1> &currentResult, const MatrixType &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) const {
        if ((A * currentResult - b).squaredNorm() / b.squaredNorm() >= tol) {
            return false;
        }
        return true;
    }


public:
    IterativeSolver(unsigned int maxIter, UpdateStrategy::Strategy<T, MatrixType>* const updateStrategy, T tol) : AbstractSolver<T, MatrixType>(), maxIter(maxIter), updateStrategy(updateStrategy), tol(tol) {}
    IterativeSolver(const IterativeSolver<T, MatrixType> &other) : updateStrategy(nullptr) {
        if (other.updateStrategy != nullptr) {
            this->updateStrategy = other.updateStrategy->clone();
        }
        this->maxIter = other.maxIter;
        this->iterations = other.iterations;
        this->tol = other.tol;
    }
    unsigned int neededIterations() const {
        return this->iterations;
    }

    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>

    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
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

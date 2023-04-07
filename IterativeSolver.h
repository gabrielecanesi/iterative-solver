#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "Solver.h"
#include "UpdateStrategy.h"

template<typename T, typename MatrixType>
class IterativeSolver : AbstractSolver<T, MatrixType> {

private:
    unsigned int maxIter;
    unsigned int iterations;
    UpdateStrategy<T, MatrixType>* const updateStrategy;

    bool reachedTolerance(const Eigen::Matrix<T, Eigen::Dynamic, 1> &currentResult, const MatrixType &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) const {
        if ((A * currentResult - b).squaredNorm() / b.squaredNorm() >= tol) {
            return false;
        }
        return true;
    }


public:
    IterativeSolver(unsigned int maxIter, UpdateStrategy<T, MatrixType>* const updateStrategy) : AbstractSolver<T, MatrixType>(), maxIter(maxIter), updateStrategy(updateStrategy) {}
    unsigned int neededIterations() const {
        return this->iterations;
    }

    virtual ~IterativeSolver() {
        delete updateStrategy;
    }

    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>

    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) override {
        updateStrategy->init(A, b);
        const Eigen::Matrix<T, Eigen::Dynamic, 1> *currentResult;
        unsigned int iter = 0;

        do {
            currentResult = updateStrategy->update();
            ++iter;
        } while (iter <= maxIter && !reachedTolerance(*currentResult, A, b, tol));

        this->iterations = iter;
        return *currentResult;
    }

};

#endif
#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "Solver.h"
#include "UpdateStrategy.h"

template<typename T>
class IterativeSolver : AbstractSolver<T> {
    private:
    unsigned int maxIter;
    unsigned int iterations;

    bool reachedTolerance(const Eigen::Matrix<T, Eigen::Dynamic, 1> &currentResult, const Eigen::SparseMatrix<T> &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) const {
        if ((A * currentResult - b).squaredNorm() / b.squaredNorm() >= tol) {
            return false;
        }
        return true;
    }


    public:
    IterativeSolver(unsigned int maxIter) : AbstractSolver<T>(), maxIter(maxIter) {}
    unsigned int neededIterations() const {
        return this->iterations;
    }
    virtual ~IterativeSolver() {}

    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>
    solve(Eigen::SparseMatrix<T> &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol, UpdateStrategy<T> &updateStrategy) override {
        updateStrategy.init(A, b);
        const Eigen::Matrix<T, Eigen::Dynamic, 1> *currentResult;
        unsigned int iter = 0;

        do {
            currentResult = updateStrategy.update();
            ++iter;
        } while (iter <= maxIter && !reachedTolerance(*currentResult, A, b, tol));

        this->iterations = iter;
        return *currentResult;
    }

};

#endif
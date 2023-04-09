#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <eigen3/Eigen/Dense>
#include "IterativeSolver.h"
#include "UpdateStrategy.h"
#include "timer.h"

template<typename Precision, typename MatrixType>
class IterativeBenchmark {
    private:
    Precision M_relativeError;
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> solution;
    IterativeSolver<Precision, MatrixType>* solver;
    Timer timer;

    public:
    IterativeBenchmark() : M_relativeError(0), solver(nullptr) {}
    ~IterativeBenchmark() {
        delete solver;
    }

    void run(MatrixType &A, Eigen::Matrix<Precision, Eigen::Dynamic, 1> &b, unsigned int maxIter, Precision tolerance, UpdateStrategy<Precision, MatrixType> &strategy,
            const Eigen::Matrix<Precision, Eigen::Dynamic, 1> &x) {
                solver = new IterativeSolver<Precision, MatrixType>(maxIter, &strategy, tolerance);
                timer.tic();
                solution = solver->solve(A, b);
                timer.toc();
                M_relativeError = (solution - x).squaredNorm() / x.squaredNorm();
    }

    unsigned int elapsedMilliseconds() {
        return timer.elapsedMilliseconds();
    }

    unsigned int neededIterations() {
        if (solver != nullptr){
            return solver->neededIterations();
        }
        return 0;
    }

    Precision relativeError() {
        return M_relativeError;
    }

};

#endif
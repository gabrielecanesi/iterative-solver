#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "Solver.h"
#include "UpdateStrategy.h"

template<typename T>
class IterativeSolver : Solver<T> {
    private:
    unsigned int maxIter;
    UpdateStrategy<T> *updateStrategy;

    public:
    IterativeSolver(unsigned int maxIter) : Solver<T>(), maxIter(maxIter), updateStrategy(nullptr) {}
    void setUpdateStrategy(UpdateStrategy<T> &strategy) {
        this->updateStrategy = &strategy;
    }

    virtual ~IterativeSolver() {
        delete updateStrategy;
    }

    virtual Eigen::Matrix<T, Eigen::Dynamic, 1> solve(const Eigen::SparseMatrix<T> &A,
                                        const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,
                                        const double tol) override {
                                            if (updateStrategy == nullptr) {
                                                throw std::runtime_error("No update strategy given.");
                                            }
                                                Eigen::Matrix<T, Eigen::Dynamic, 1> vector(A.rows(), 1);
                                                unsigned int iter = 0;
                                                while (iter <= maxIter) {
                                                    updateStrategy->update();
                                                    ++iter;
                                                }

                                                return vector;
                                        }
};

#endif
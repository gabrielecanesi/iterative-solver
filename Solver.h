#ifndef SOLVER_H
#define SOLVER_H

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

template<typename T>
class Solver {
    public:
    virtual Eigen::Matrix<T, Eigen::Dynamic, 1> solve(const Eigen::SparseMatrix<T> &A,
                                        const Eigen::Matrix<T, Eigen::Dynamic, 1> &b,
                                        const double tol) = 0;
    virtual ~Solver() {}
};



#endif
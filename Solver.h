#ifndef SOLVER_H
#define SOLVER_H

#include "UpdateStrategy.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

template<typename T, typename MatrixType>
class AbstractSolver {
public:
    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>
    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b, const double tol) = 0;
    virtual ~AbstractSolver() {}
};



#endif
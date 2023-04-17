#ifndef SOLVER_H
#define SOLVER_H

#include "updateStrategy/Strategy.h"
#include "norm.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>


template<typename T, typename MatrixType>
class AbstractSolver {
public:
    virtual Eigen::Matrix<T, Eigen::Dynamic, 1>
    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) = 0;
    virtual ~AbstractSolver() {}
};



#endif
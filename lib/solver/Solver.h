#ifndef SOLVER_H
#define SOLVER_H

#include "updateStrategy/Strategy.h"
#include "NormType.h"
#include "SolverResults.h"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <memory>


template<typename T, typename MatrixType>
class AbstractSolver {
public:
    virtual std::shared_ptr<SolverResults<T, MatrixType>>
    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) = 0;
    virtual ~AbstractSolver() {}
};



#endif
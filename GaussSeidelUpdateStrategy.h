
#ifndef ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H
#define ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

#include "./UpdateStrategy.h"
#include "BackwardSubstitutionSolver.h"

template <typename T, typename MatrixType>
class GaussSeidelUpdateStrategy: public UpdateStrategy<T, MatrixType> {

public:
    GaussSeidelUpdateStrategy(): UpdateStrategy<T, MatrixType>() {};


    const Eigen::Matrix<T, Eigen::Dynamic, 1>* const update() {
        BackwardSubstitutionSolver<T, MatrixType> solver;

        Eigen::Matrix<T, Eigen::Dynamic, 1> residual = compute_residual();
        this->result = this->result - solver.solve(*this->A, residual);
        return &this->result;
    }


    const Eigen::Matrix<T, Eigen::Dynamic, 1> compute_residual() const {
        return (*this->A * this->result) - *this->b;
    }

};

#endif //ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

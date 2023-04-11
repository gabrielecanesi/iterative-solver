
#ifndef ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H
#define ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

#include "./Strategy.h"
#include "solver/BackwardSubstitutionSolver.h"

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class GaussSeidelUpdateStrategy : public Strategy<T, MatrixType> {

    public:
        GaussSeidelUpdateStrategy() : Strategy<T, MatrixType>() {};


        const Eigen::Matrix<T, Eigen::Dynamic, 1> *const update() override {
            BackwardSubstitutionSolver<T, MatrixType> solver;

            Eigen::Matrix<T, Eigen::Dynamic, 1> residual = compute_residual();
            this->result = this->result - solver.solve(*this->A, residual);
            return &this->result;
        }


        const Eigen::Matrix<T, Eigen::Dynamic, 1> compute_residual() const {
            return (*this->A * this->result) - *this->b;
        }

        virtual std::string name() const override {
            return "Gauss-Seidel";
        }

        virtual Strategy<T, MatrixType> *clone() override {
            GaussSeidelUpdateStrategy<T, MatrixType> *ret = new GaussSeidelUpdateStrategy<T, MatrixType>();
            ret->A = this->A;
            ret->b = this->b;
            ret->result = this->result;

            return ret;
        }
    };

}

#endif //ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

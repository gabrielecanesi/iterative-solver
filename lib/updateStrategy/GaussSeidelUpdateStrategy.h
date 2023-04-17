
#ifndef ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H
#define ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

#include "./Strategy.h"
#include "solver/BackwardSubstitutionSolver.h"

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class GaussSeidelUpdateStrategy : public Strategy<T, MatrixType> {

    public:
        GaussSeidelUpdateStrategy(T w) : Strategy<T, MatrixType>(), P(nullptr), w(w) {};
        GaussSeidelUpdateStrategy() : Strategy<T, MatrixType>(), P(nullptr), w(1) {};

        virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
            Strategy<T, MatrixType>::init(A, b);
            P = new MatrixType();
            *P = *this->A;
            P->diagonal() /= w;
        }

        const Eigen::Matrix<T, Eigen::Dynamic, 1> *const update() override {
            BackwardSubstitutionSolver<T, MatrixType> solver;

            Eigen::Matrix<T, Eigen::Dynamic, 1> residual = compute_residual();


            this->result = this->result - solver.solve(*P, residual);
            return &this->result;
        }

        virtual Strategy<T, MatrixType> *clone() override {
            GaussSeidelUpdateStrategy<T, MatrixType> *ret = new GaussSeidelUpdateStrategy<T, MatrixType>();
            ret->w = this->w;
            ret->P = P;
            ret->A = this->A;
            ret->b = this->b;
            ret->result = this->result;

            return ret;
        }

        virtual std::string name() const override {
            return "Gauss-Seidel";
        }

    private:
        MatrixType *P;
        T w;

        const Eigen::Matrix<T, Eigen::Dynamic, 1> compute_residual() const {
            return (*this->A * this->result) - *this->b;
        }
    };

}

#endif //ITERATIVE_SOLVER_GAUSSSEIDELUPDATESTRATEGY_H

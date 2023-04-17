#ifndef JACOBI_UPDATE_STRATEGY_H
#define JACOBI_UPDATE_STRATEGY_H

#include "solver/IterativeSolver.h"

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class JacobiUpdateStrategy : public Strategy<T, MatrixType> {

    public:
        JacobiUpdateStrategy() : Strategy<T, MatrixType>() {}
        JacobiUpdateStrategy(T w) : Strategy<T, MatrixType>(), w(w) {}

        virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
            Strategy<T, MatrixType>::init(A, b);
            PInverse = new Eigen::DiagonalMatrix<T, Eigen::Dynamic>(this->A->diagonal());
            *PInverse = PInverse->inverse() * w;
        }

        friend class IterativeSolver<T, MatrixType>;

    private:
        Eigen::DiagonalMatrix<T, Eigen::Dynamic> *PInverse;
        T w;

        const Eigen::Matrix<T, Eigen::Dynamic, 1> *const update() override {

            Eigen::Matrix<T, Eigen::Dynamic, 1> r = (*this->A * this->result) - *this->b;
            this->result -= *PInverse * r;
            return &this->result;
        }

        virtual std::string name() const override {
            return "Jacobi";
        }

        virtual Strategy<T, MatrixType> *clone() override {
            JacobiUpdateStrategy<T, MatrixType> *ret = new JacobiUpdateStrategy<T, MatrixType>();
            ret->w = w;
            ret->PInverse = PInverse;
            ret->A = this->A;
            ret->b = this->b;
            ret->result = this->result;

            return ret;
        }

    };

}

#endif
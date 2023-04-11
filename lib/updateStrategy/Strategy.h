#ifndef UPDATE_STRATEGY_H
#define UPDATE_STRATEGY_H
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "solver/IterativeSolver.h"

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class Strategy {
    protected:
        Eigen::Matrix<T, Eigen::Dynamic, 1> result;
        Eigen::Matrix<T, Eigen::Dynamic, 1> *b;
        MatrixType *A;


    public:
        virtual const Eigen::Matrix<T, Eigen::Dynamic, 1> *const update() = 0;

        virtual ~Strategy() {}

        virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {
            this->A = &A;
            this->b = &b;
            result = Eigen::Matrix<T, Eigen::Dynamic, 1>(this->A->cols(), 1);
            result.setZero();
        }

        virtual std::string name() const = 0;

        virtual Strategy<T, MatrixType> *clone() = 0;
    };

}

#endif

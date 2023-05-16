#ifndef UPDATE_STRATEGY_H
#define UPDATE_STRATEGY_H
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "solver/IterativeSolver.h"
#include <memory>

namespace UpdateStrategy
{

    template <typename T, typename MatrixType>
    class Strategy
    {
    protected:
        std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> result;
        Eigen::Matrix<T, Eigen::Dynamic, 1> *b;
        MatrixType *A;

    public:
        virtual const std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> update() = 0;

        virtual ~Strategy() {}

        Strategy() {}
        Strategy(const Strategy &other)
        {
            this->A = other.A;
            this->b = other.b;
            this->result = other.result;
        }

        virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b)
        {
            this->A = &A;
            this->b = &b;
            result = std::make_shared<Eigen::Matrix<T, Eigen::Dynamic, 1>>(this->A->cols(), 1);
            result.get()->setZero();
        }

        virtual std::string name() const = 0;

        virtual std::shared_ptr<UpdateStrategy::Strategy<T, MatrixType>> clone() = 0;
    };

}

#endif

#ifndef UPDATE_STRATEGY_H
#define UPDATE_STRATEGY_H
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include "IterativeSolver.h"

template<typename T, typename MatrixType>
class UpdateStrategy {
    protected:
Eigen::Matrix<T, Eigen::Dynamic, 1> result;
Eigen::Matrix<T, Eigen::Dynamic, 1> *b;
MatrixType *A;

    
    public:
    virtual const Eigen::Matrix<T, Eigen::Dynamic, 1>* const update() = 0;
    virtual ~UpdateStrategy(){}

    virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {
        this->A = &A;
        this->b = &b;
        result = Eigen::Matrix<T, Eigen::Dynamic, 1>(this->A->cols(), 1);
        result.setZero();
    }

    virtual std::string name() const = 0;
    virtual UpdateStrategy<T, MatrixType>* clone() = 0;
};


#endif
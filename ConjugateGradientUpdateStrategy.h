#ifndef CONJUGATE_GRADIENT_UPDATE_STRATEGY_H
#define CONJUGATE_GRADIENT_UPDATE_STRATEGY_H

#include "IterativeSolver.h"

template<typename T, typename MatrixType>
class ConjugateGradientUpdateStrategy : public UpdateStrategy<T, MatrixType> {
    private:
    Eigen::Matrix<T, Eigen::Dynamic, 1> *d;
    Eigen::Matrix<T, Eigen::Dynamic, 1> *residual;

    public:
    ConjugateGradientUpdateStrategy() : UpdateStrategy<T, MatrixType>(),
                d(new Eigen::Matrix<T, Eigen::Dynamic, 1>()), residual(new Eigen::Matrix<T, Eigen::Dynamic, 1>()){}

    virtual ~ConjugateGradientUpdateStrategy(){
        delete d;
        delete residual;
    }

    friend class IterativeSolver<T, MatrixType>;
    private:
    const Eigen::Matrix<T, Eigen::Dynamic, 1>* const update() override {
        T alpha = computeAlpha(*residual);
        this->result += alpha * *d;
        *residual = computeResidual();
        updateD(*residual);
        return &this->result;
    }

    void updateD(const Eigen::Matrix<T, Eigen::Dynamic, 1> &residual) {
        T beta = computeBeta(residual);
        *d = residual - beta * *d;
    }    

    T computeBeta(const Eigen::Matrix<T, Eigen::Dynamic, 1> &residual) {
        T numerator = d->transpose() * *this->A * residual;
        T denominator = d->transpose() * *this->A * *this->d;
        return numerator / denominator;
    }

    T computeAlpha(const Eigen::Matrix<T, Eigen::Dynamic, 1> &residual) const {
        Eigen::Matrix<T, 1, Eigen::Dynamic> transpose = d->transpose();
        T numerator = transpose * residual;
        T denominator = transpose * *this->A * *d;
        return numerator / denominator;
    }
    
    Eigen::Matrix<T, Eigen::Dynamic, 1> computeResidual() const {
        return *this->b - (*this->A * this->result);
    }

    virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
        UpdateStrategy<T, MatrixType>::init(A, b);
        *residual = computeResidual();
        *d = *residual;
    }
};

#endif
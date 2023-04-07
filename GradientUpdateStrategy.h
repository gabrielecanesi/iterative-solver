#ifndef GRADIENT_UPDATE_STRATEGY_H
#define GRADIENT_UPDATE_STRATEGY_H

#include "IterativeSolver.h"

template<typename T>
class GradientUpdateStrategy : public UpdateStrategy<T> {
    
    public:
    GradientUpdateStrategy() : UpdateStrategy<T>() {}

    friend class IterativeSolver<T>;
    private:
    const Eigen::Matrix<T, Eigen::Dynamic, 1>* const update() override {
        Eigen::Matrix<T, Eigen::Dynamic, 1> residual = computeResidual();
        T alpha = computeAlpha(residual);
        this->result += alpha * residual;
        return &this->result;
    }


    T computeAlpha(const Eigen::Matrix<T, Eigen::Dynamic, 1> &residual) const {
        Eigen::Matrix<T, 1, Eigen::Dynamic> transpose = residual.transpose();
        T numerator = transpose * residual;
        T denominator = transpose * *this->A * residual;
        return numerator / denominator;
    }
    
    Eigen::Matrix<T, Eigen::Dynamic, 1> computeResidual() const {
        return *this->b - (*this->A * this->result);
    }

};

#endif
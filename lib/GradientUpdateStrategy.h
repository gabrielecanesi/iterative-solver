#ifndef GRADIENT_UPDATE_STRATEGY_H
#define GRADIENT_UPDATE_STRATEGY_H

#include "IterativeSolver.h"

template<typename T, typename MatrixType>
class GradientUpdateStrategy : public UpdateStrategy<T, MatrixType> {
    
    public:
    GradientUpdateStrategy() : UpdateStrategy<T, MatrixType>() {}

    friend class IterativeSolver<T, MatrixType>;
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

    virtual std::string name() const override {
        return "Gradient";
    }

    virtual UpdateStrategy<T, MatrixType>* clone() override {
        GradientUpdateStrategy<T, MatrixType>* ret = new GradientUpdateStrategy<T, MatrixType>();
        ret->A = this->A;
        ret->b = this->b;
        ret->result = this->result;
        
        return ret;
    }
};

#endif
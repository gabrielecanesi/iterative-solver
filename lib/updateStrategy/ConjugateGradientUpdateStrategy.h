#ifndef CONJUGATE_GRADIENT_UPDATE_STRATEGY_H
#define CONJUGATE_GRADIENT_UPDATE_STRATEGY_H

#include "solver/IterativeSolver.h"

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class ConjugateGradientUpdateStrategy : public Strategy<T, MatrixType> {
    private:
        Eigen::Matrix<T, Eigen::Dynamic, 1> *d;
        Eigen::Matrix<T, Eigen::Dynamic, 1> *residual;

    public:
        ConjugateGradientUpdateStrategy() : Strategy<T, MatrixType>(),
                                            d(new Eigen::Matrix<T, Eigen::Dynamic, 1>()),
                                            residual(new Eigen::Matrix<T, Eigen::Dynamic, 1>()) {}

        virtual ~ConjugateGradientUpdateStrategy() {
            delete d;
            delete residual;
        }

        ConjugateGradientUpdateStrategy(const ConjugateGradientUpdateStrategy &strategy) : Strategy<T, MatrixType>(strategy) {
            this->d = new Eigen::Matrix<T, Eigen::Dynamic, 1>(*strategy.d);
            this->residual = new Eigen::Matrix<T, Eigen::Dynamic, 1>(*strategy.d);
        }

        friend class IterativeSolver<T, MatrixType>;

    private:
        const Eigen::Matrix<T, Eigen::Dynamic, 1> *const update() override {
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
            Strategy<T, MatrixType>::init(A, b);
            *residual = computeResidual();
            *d = *residual;
        }

        virtual std::string name() const override {
            return "Conjugate gradient";
        }

        virtual Strategy<T, MatrixType> *clone() override {
            return new ConjugateGradientUpdateStrategy<T, MatrixType>(*this);
        }
    };
}

#endif
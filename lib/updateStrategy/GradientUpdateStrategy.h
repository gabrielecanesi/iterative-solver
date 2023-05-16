#ifndef GRADIENT_UPDATE_STRATEGY_H
#define GRADIENT_UPDATE_STRATEGY_H

#include <updateStrategy/Strategy.h>

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class GradientUpdateStrategy : public Strategy<T, MatrixType> {

    public:
        GradientUpdateStrategy() : Strategy<T, MatrixType>() {}
        GradientUpdateStrategy(const GradientUpdateStrategy& other) : Strategy<T, MatrixType>(other) {}

    private:
        const std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> update() override {
            Eigen::Matrix<T, Eigen::Dynamic, 1> residual = computeResidual();
            T alpha = computeAlpha(residual);
            *this->result.get() += alpha * residual;
            return this->result;
        }


        T computeAlpha(const Eigen::Matrix<T, Eigen::Dynamic, 1> &residual) const {
            Eigen::Matrix<T, 1, Eigen::Dynamic> transpose = residual.transpose();
            T numerator = transpose * residual;
            T denominator = transpose * *this->A * residual;
            return numerator / denominator;
        }

        Eigen::Matrix<T, Eigen::Dynamic, 1> computeResidual() const {
            return *this->b - (*this->A * *this->result.get());
        }

        virtual std::string name() const override {
            return "Gradient";
        }

        virtual std::shared_ptr<UpdateStrategy::Strategy<T, MatrixType>> clone() override {
            return std::make_shared<GradientUpdateStrategy<T, MatrixType>>(*this);
        }
    };

}

#endif
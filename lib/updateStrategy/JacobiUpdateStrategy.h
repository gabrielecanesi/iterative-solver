#ifndef JACOBI_UPDATE_STRATEGY_H
#define JACOBI_UPDATE_STRATEGY_H

#include <updateStrategy/Strategy.h>
#include <exceptions/WrongParameterValueException.h>

namespace UpdateStrategy {

    template<typename T, typename MatrixType>
    class JacobiUpdateStrategy : public Strategy<T, MatrixType> {

    public:
        JacobiUpdateStrategy() : Strategy<T, MatrixType>() {}
        JacobiUpdateStrategy(T w) : Strategy<T, MatrixType>(), w(w) {
            if (w <= 0 || w > 1) {
                throw WrongParameterValueException(0, 1, false, true);
            }
        }

        JacobiUpdateStrategy(const JacobiUpdateStrategy &other) : Strategy<T, MatrixType>() {
            this->w = other.w;
            this->PInverse = other.PInverse;
        }

        virtual void init(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
            Strategy<T, MatrixType>::init(A, b);
            PInverse = new Eigen::DiagonalMatrix<T, Eigen::Dynamic>(this->A->diagonal());
            *PInverse = PInverse->inverse() * w;
        }

    private:
        Eigen::DiagonalMatrix<T, Eigen::Dynamic> *PInverse;
        T w;

        const std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> update() override {

            Eigen::Matrix<T, Eigen::Dynamic, 1> &r = this->residual;
            *this->result.get() += *PInverse * r;
            return this->result;
        }

        virtual std::string name() const override {
            return "Jacobi";
        }

        virtual std::shared_ptr<UpdateStrategy::Strategy<T, MatrixType>> clone() override {
            return std::make_shared<JacobiUpdateStrategy<T, MatrixType>>(*this);
        }

    };

}

#endif
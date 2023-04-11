#ifndef JACOBI_UPDATE_STRATEGY_H
#define JACOBI_UPDATE_STRATEGY_H

#include "IterativeSolver.h"

template<typename T, typename MatrixType>
class JacobiUpdateStrategy : public UpdateStrategy<T, MatrixType> {

	public:
    JacobiUpdateStrategy() : UpdateStrategy<T, MatrixType>() {}
	friend class IterativeSolver<T, MatrixType>;
    
    private:
    const Eigen::Matrix<T, Eigen::Dynamic, 1>* const update() override {

	    Eigen::DiagonalMatrix<T, Eigen::Dynamic> p(this->A->diagonal());

	    Eigen::Matrix<T, Eigen::Dynamic, 1> r = *this->b - (*this->A * this->result);

	    this->result += p.inverse() * r;
	    return &this->result;

    }

	virtual std::string name() const override {
        return "Jacobi";
    }

	virtual UpdateStrategy<T, MatrixType>* clone() override {
        JacobiUpdateStrategy<T, MatrixType>* ret = new JacobiUpdateStrategy<T, MatrixType>();
        ret->A = this->A;
        ret->b = this->b;
        ret->result = this->result;
        
        return ret;
    }

};

#endif
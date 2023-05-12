#ifndef ITERATIVE_SOLVER_RESULT_H
#define ITERATIVE_SOLVER_RESULT_H

#include <Eigen/Dense>
#include "updateStrategy/Strategy.h"
#include "NormType.h"
#include "SolverResults.h"

template <typename Precision, typename MatrixType>
class IterativeSolverResult : public SolverResults<Precision, MatrixType> {
    private:
    unsigned int M_neededIterations;
    UpdateStrategy::Strategy<Precision, MatrixType> *M_usedStrategy;
    NormType M_normType;

    public:
    IterativeSolverResult(const Eigen::Matrix<Precision, Eigen::Dynamic, 1> *solution, Precision conditionNumber,
                            UpdateStrategy::Strategy<Precision, MatrixType> *usedStrategy,
                            NormType normType) : SolverResults<Precision, MatrixType>(solution, conditionNumber), M_usedStrategy(usedStrategy), M_normType(normType){}

    IterativeSolverResult(const IterativeSolverResult &other) : SolverResults<Precision, MatrixType>(other),
    M_neededIterations(other.M_neededIterations), M_usedStrategy(other.M_usedStrategy),
    M_normType(other.M_normType) {}

    UpdateStrategy::Strategy<Precision, MatrixType> *usedStrategy() const {
        return M_usedStrategy;
    }


    unsigned int neededIterations() const {
        return M_neededIterations;
    }

    NormType normType() const {
        return M_normType;
    }

};

#endif
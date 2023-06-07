#ifndef ITERATIVE_SOLVER_H
#define ITERATIVE_SOLVER_H

#include "IterativeSolverResult.h"
#include "Solver.h"
#include "updateStrategy/Strategy.h"
#include <Eigen/Cholesky>
#include "exceptions/NonSymmetricAndPositiveDefiniteException.h"
#include "NormType.h"
#include "exceptions/NonSquareMatrixException.h"
#include "constants.h"
#include "MatrixUtil.h"
#include <memory>

template<typename T, typename MatrixType>
class IterativeSolver : AbstractSolver<T, MatrixType> {

private:
    unsigned int maxIter;
    unsigned int iterations;
    std::shared_ptr<UpdateStrategy::Strategy<T, MatrixType>> updateStrategy;
    T tol;
    bool skipMatrixCheck = false;
    NormType normType;
    bool skipCondition;
    T bNorm;

    void computeBNorm(const Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {
        switch (normType) {
            case NormType::EUCLIDEAN:
                bNorm = b.norm();
                break;
            case NormType::MANHATTAN:
                bNorm = b.template lpNorm<1>();
                break;
            case NormType::INFTY:
                bNorm = b.template lpNorm<Eigen::Infinity>();
                break;
            default:
                break;
        }
    }

    bool reachedTolerance(const Eigen::Matrix<T, Eigen::Dynamic, 1> &b, T tol) const {
        double abNorm;
        
        switch (normType) {
            case NormType::EUCLIDEAN:
                abNorm = (updateStrategy->getResidual()).norm();
                break;
            case NormType::MANHATTAN:
                abNorm = (updateStrategy->getResidual()).template lpNorm<1>();
                break;
            case NormType::INFTY:
                abNorm = (updateStrategy->getResidual()).template lpNorm<Eigen::Infinity>();
                break;
            default:
                break;
        }
        
        return (abNorm / bNorm) < tol;
    }

    T updateResidual(const Eigen::Matrix<T, Eigen::Dynamic, 1> &currentResult, const MatrixType &A, const Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {
        updateStrategy->getResidual() = b - (A * currentResult);
    }


public:
    IterativeSolver(unsigned int maxIter, std::shared_ptr<UpdateStrategy::Strategy<T, MatrixType>> updateStrategy, T tol, bool skipMatrixCheck, NormType normType = NormType::EUCLIDEAN, bool skipCondition = false) : AbstractSolver<T, MatrixType>(),
                                                                                                                                        maxIter(maxIter),
                                                                                                                                        updateStrategy(updateStrategy),
                                                                                                                                        tol(tol),
                                                                                                                                        skipMatrixCheck(skipMatrixCheck),
                                                                                                                                        normType(normType),
                                                                                                                                        skipCondition(skipCondition){}
                                                                                                                                        
    IterativeSolver(const IterativeSolver<T, MatrixType> &other) : updateStrategy(nullptr) {
        if (other.updateStrategy != nullptr) {
            this->updateStrategy = other.updateStrategy->clone();
        }
        this->maxIter = other.maxIter;
        this->iterations = other.iterations;
        this->tol = other.tol;
        this->skipMatrixCheck = other.skipMatrixCheck;
    }

    unsigned int neededIterations() const {
        return this->iterations;
    }

    virtual std::shared_ptr<SolverResults<T, MatrixType>>
    solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
        if (A.rows() != A.cols()) {
            throw NonSquareMatrixException();
        }

        T cond = -1;
        
        if (!skipCondition) {
            cond = MatrixUtil::conditionNumber<T, MatrixType>(A, 1e-6, 1000);
        }

        computeBNorm(b);

        if (b.isZero(ZERO_THRESHOLD)) {
            std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> solution = std::make_shared<Eigen::Matrix<T, Eigen::Dynamic, 1>>(b.rows(), 1);
            solution.get()->setZero();
            auto sol = std::make_shared<IterativeSolverResult<T, MatrixType>>(solution, cond, updateStrategy, normType);
            return sol;
        }

        if(!skipMatrixCheck) {
            MatrixUtil::checkSymmetricAndPositiveDefinite(A);
        }

        updateStrategy->init(A, b);
        std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> currentResult = updateStrategy->getResult();
        updateResidual(*currentResult, A, b);
        unsigned int iter = 0;

        do {
            currentResult = updateStrategy->update();
            updateResidual(*currentResult, A, b);
            ++iter;
        } while (iter < maxIter && !reachedTolerance(b, tol));

        this->iterations = iter;

        return std::make_shared<IterativeSolverResult<T, MatrixType>>(currentResult, cond, updateStrategy, normType);
    }

    std::string methodName() const {
        if (updateStrategy == nullptr) {
            return "";
        }
        return updateStrategy->name();
    }

    T tolerance() const {
        return tol;
    }
};

#endif

#ifndef ITERATIVE_SOLVER_BACKWARDSUBSTITUTIONSOLVER_H
#define ITERATIVE_SOLVER_BACKWARDSUBSTITUTIONSOLVER_H

#include "./Solver.h"

template<typename T, typename MatrixType>
class BackwardSubstitutionSolver: public AbstractSolver<T, MatrixType> {

public:
    Eigen::Matrix<T, Eigen::Dynamic, 1> solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
        const int n = b.rows();
        Eigen::Matrix<T, Eigen::Dynamic, 1> currentResult(n, 1);

        currentResult(n - 1, 0) = b(n - 1) / A.coeff(n - 1, n - 1);

        for(int i = n - 1; i >= 0; --i) {
            T summation = 0;
            for(int j = i + 1; j < n; ++j)
                summation += A.coeff(i, j) * currentResult.coeff(j);

            currentResult(i, 0) = (b.coeff(i) - summation) / A.coeff(i, i);
        }

        return currentResult;
    }

};

#endif //ITERATIVE_SOLVER_BACKWARDSUBSTITUTIONSOLVER_H

#ifndef ITERATIVE_SOLVER_FORWARDSUBSTITUTIONSOLVER_H
#define ITERATIVE_SOLVER_FORWARDSUBSTITUTIONSOLVER_H

#include "./Solver.h"

template<typename T, typename MatrixType>
class ForwardSubstitutionSolver: public AbstractSolver<T, MatrixType> {

public:
    std::shared_ptr<SolverResults<T, MatrixType>> solve(MatrixType &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) override {
        std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> solution = solveSpecific(A, b);
        return std::make_shared<SolverResults<T, MatrixType>>(solution, 0);
    }

private:

     std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> solveSpecific(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {
        
        const int n = b.rows();
        std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> currentResult = std::make_shared<Eigen::Matrix<T, Eigen::Dynamic, 1>>(n, 1);

        (*currentResult)(0, 0) = b(0) / A.coeff(0, 0);

        for(int i = 1; i < n; ++i) {
            T summation = 0;
            for(int j = 0; j < i; ++j)
                summation += A.coeff(i, j) * (*currentResult).coeff(j);

            (*currentResult)(i, 0) = (b.coeff(i) - summation) / A.coeff(i, i);
        }

        return currentResult;
    }

     std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> solveSpecific(Eigen::SparseMatrix<T> &A, Eigen::Matrix<T, Eigen::Dynamic, 1> &b) {

        const int n = b.rows();
        std::shared_ptr<Eigen::Matrix<T, Eigen::Dynamic, 1>> currentResult = std::make_shared<Eigen::Matrix<T, Eigen::Dynamic, 1>>(n, 1);
        Eigen::Matrix<T, Eigen::Dynamic, 1> summation(n, 1);
        summation.fill(0);
        int currentCol = 0;
        T currentCoeff = b(0) / A.coeff(0, 0);
        (*currentResult)(0) =  currentCoeff;

        Eigen::SparseMatrix<T> lowerDiagonal = A.template triangularView<Eigen::Lower>();

        for (int k = 0; k < lowerDiagonal.outerSize(); ++k) {
            for (typename Eigen::SparseMatrix<T>::InnerIterator it(lowerDiagonal, k); it; ++it) {

                if (it.row() != it.col()) //check if the element is not on the diagonal
                    summation(it.row()) += it.value() * currentCoeff;

                if(currentCol != it.col()) {
                    currentCoeff = (b.coeff(it.col()) - summation(it.col())) / A.coeff(it.col(), it.col());
                    (*currentResult)(it.col()) = currentCoeff;
                }

                currentCol = it.col();
            }
        }

        return currentResult;

    }

};

#endif //ITERATIVE_SOLVER_FORWARDSUBSTITUTIONSOLVER_H

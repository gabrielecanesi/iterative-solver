#include <iostream>
#include "IterativeSolver.h"

#include "ConjugateGradientUpdateStrategy.h"
#include "Solver.h"
#include "GradientUpdateStrategy.h"

#include "Solver.h"
#include "JacobiUpdateStrategy.h"
#include "MatrixReader.h"

typedef float precision;


template<typename T, typename MatrixType>
Eigen::Matrix<T, Eigen::Dynamic, 1> solve(UpdateStrategy<T, MatrixType> &updateStrategy, MatrixType &A, Eigen::Matrix<precision, Eigen::Dynamic, 1> b,
Eigen::Matrix<precision, Eigen::Dynamic, 1> &x) {
    IterativeSolver<precision, Eigen::SparseMatrix<precision>> solver(500);
    auto foundSolution = solver.solve(A, b, 10e-10, updateStrategy);

    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Relative error: " << (foundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;
    return foundSolution;
}


int main() {
    std::string filename = "./Matrices/spa2.mtx";
    Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>(filename);
    Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;

    GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>> gradientUpdateStrategy;
    ConjugateGradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>> ConjugateGradientUpdateStrategy;
    solve(gradientUpdateStrategy, A, b, x);
    solve(ConjugateGradientUpdateStrategy, A, b, x);
    std::cout << "Non zero proportion: " << MatrixReader::nonZeroProportion(filename) * 100 << "%" << std::endl;


    return 0;
}

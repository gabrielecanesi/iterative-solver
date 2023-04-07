#include <iostream>
#include "IterativeSolver.h"

#include "GradientUpdateStrategy.h"
#include "JacobiUpdateStrategy.h"
#include "MatrixReader.h"

typedef double precision;
int main() {

    Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>("./Matrices/spa1.mtx");
    Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;
    IterativeSolver<precision, Eigen::SparseMatrix<precision>> solver(5000);

    GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>> Gstrategy;
    auto GfoundSolution = solver.solve(A, b, 10e-10, Gstrategy);
    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Gradient-based relative error: " << (GfoundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;

    JacobiUpdateStrategy<precision, Eigen::SparseMatrix<precision>> Jstrategy;
    auto JfoundSolution = solver.solve(A, b, 10e-10, Jstrategy);
    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Jacobi-based relative error: " << (JfoundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;

    return 0;
}

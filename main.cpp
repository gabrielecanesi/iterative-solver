#include <iostream>
#include "IterativeSolver.h"

#include "GradientUpdateStrategy.h"
#include "MatrixReader.h"

typedef double precision;
int main() {

    Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>("./Matrices/spa1.mtx");
    Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;

    auto strategy = new GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>>;
    IterativeSolver<precision, Eigen::SparseMatrix<precision>> solver(5000, strategy);
    auto foundSolution = solver.solve(A, b, 10e-10);

    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Gradient-based relative error: " << (foundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;
    
    return 0;
}
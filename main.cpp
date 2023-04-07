#include <iostream>
#include "IterativeSolver.h"
#include "Solver.h"
#include "MatrixReader.h"

#include "ConjugateGradientUpdateStrategy.h"
#include "GradientUpdateStrategy.h"
#include "GaussSeidelUpdateStrategy.h"
#include "JacobiUpdateStrategy.h"




typedef double precision;


template<typename T, typename MatrixType>
Eigen::Matrix<T, Eigen::Dynamic, 1> solve(UpdateStrategy<T, MatrixType> &updateStrategy, MatrixType &A, Eigen::Matrix<precision, Eigen::Dynamic, 1> b,
Eigen::Matrix<precision, Eigen::Dynamic, 1> &x) {
    IterativeSolver<precision, Eigen::SparseMatrix<precision>> solver(20000, &updateStrategy,  10e-10);
    auto foundSolution = solver.solve(A, b);

    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Relative error: " << (foundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;
    return foundSolution;
}

void testMethods() {
    std::string filename = "./Matrices/spa2.mtx";
    Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>(filename);
    Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;

    std::vector<UpdateStrategy<precision, Eigen::SparseMatrix<precision>>*> methods;
    GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>> gradientUpdateStrategy;
    ConjugateGradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>> conjugateGradientUpdateStrategy;
    JacobiUpdateStrategy<precision, Eigen::SparseMatrix<precision>> jacobiUpdateStrategy;
    GaussSeidelUpdateStrategy<precision, Eigen::SparseMatrix<precision>> gaussSeidelUpdateStrategy;
    methods.push_back(&gradientUpdateStrategy);
    methods.push_back(&conjugateGradientUpdateStrategy);
    methods.push_back(&jacobiUpdateStrategy);
    methods.push_back(&gaussSeidelUpdateStrategy);


    for (UpdateStrategy<precision, Eigen::SparseMatrix<precision>> *strategy : methods) {
        solve(*strategy, A, b, x);
    }
    std::cout << "Non zero proportion: " << MatrixReader::nonZeroProportion(filename) * 100 << "%" << std::endl;
}

int main() {
    testMethods();
    return 0;
}

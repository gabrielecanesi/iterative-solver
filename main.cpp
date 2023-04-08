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
Eigen::Matrix<precision, Eigen::Dynamic, 1> &x, precision tolerance) {
    IterativeSolver<precision, Eigen::SparseMatrix<precision>> solver(20000, &updateStrategy,  tolerance);
    auto foundSolution = solver.solve(A, b);

    std::cout << "Stopped after " << solver.neededIterations() << " iterations." << std::endl;
    std::cout << "Relative error: " << (foundSolution - x).squaredNorm() / x.squaredNorm() << std::endl;
    return foundSolution;
}

void testMethods() {
    std::vector<precision> testTolerances = {10e-4, 10e-6, 10e-8, 10e-10}; 
    std::string filename = "./Matrices/spa2.mtx";
    Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>(filename);
    Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;

    std::vector<std::shared_ptr<UpdateStrategy<precision, Eigen::SparseMatrix<precision>>>> methods {
        std::make_shared<GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
        std::make_shared<ConjugateGradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
        std::make_shared<JacobiUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
        std::make_shared<GaussSeidelUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
    };

    for (precision tol : testTolerances) {
        std::cout << "Tolerance: " << tol << std::endl;
        for (auto &strategyPointer : methods) {
            solve(*strategyPointer.get(), A, b, x, tol);
        }
        std::cout << std::endl;
    }
    std::cout << "Non zero proportion: " << MatrixReader::nonZeroProportion(filename) * 100 << "%" << std::endl;
}

int main() {
    testMethods();
    return 0;
}

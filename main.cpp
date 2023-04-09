#include <iostream>
#include "IterativeSolver.h"
#include "Solver.h"
#include "MatrixReader.h"

#include "ConjugateGradientUpdateStrategy.h"
#include "GradientUpdateStrategy.h"
#include "GaussSeidelUpdateStrategy.h"
#include "JacobiUpdateStrategy.h"
#include "Benchmark.h"
#include <memory>

typedef double precision;


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

    IterativeBenchmark<precision, Eigen::SparseMatrix<precision>> benchmark;

    for (precision tol : testTolerances) {
        std::cout << "------------- Tolerance: " << tol << " -------------" << std::endl;
        for (auto &strategyPointer : methods) {
            benchmark.run(A, b, 20000, tol, *strategyPointer.get(), x);
            std::cout << "---- Method: " << strategyPointer.get()->name() << std::endl;
            std::cout << "\tElapsed: " << benchmark.elapsedMilliseconds() << "ms" << std::endl;
            std::cout << "\tStopped after " << benchmark.neededIterations() << " iterations." << std::endl;
            std::cout << "\tRelative error: " << benchmark.relativeError() << std::endl << std::endl;;
        }
        std::cout << std::endl;
    }
    std::cout << "Non zero proportion: " << MatrixReader::nonZeroProportion(filename) * 100 << "%" << std::endl;
}

int main() {
    testMethods();
    return 0;
}

#include <iostream>

#include <Eigen/Sparse>
#include "../runner.h"
#include <util/util.h>
#include "exceptions/NonSimmetricAndPositiveDefiniteException.h"

typedef double precision;
//typedef Eigen::Matrix<precision, Eigen::Dynamic, Eigen::Dynamic> matrix_type;
typedef Eigen::SparseMatrix<precision> matrix_type;

void testMethods() {
    std::string filename = "../Matrices/spa1.mtx";
    std::vector<IterativeBenchmark<precision, matrix_type>> results = testMethods<precision>(filename);
   
    for (auto &method : results) {
        std::cout << method.methodName() << std::endl;
        std::cout << "\tTolerance: " << method.tolerance() << std::endl;
        std::cout << "\tElapsed: " << method.elapsedMilliseconds() << std::endl;
        std::cout << "\tIterations: " << method.neededIterations() << std::endl; 
        std::cout << "\tRelative error: " << method.relativeError() << std::endl << std::endl;
    }

    std::cout << "Matrix non-zero elements: " << MatrixReader::nonZeroProportion(filename) * 100 << "%" << std::endl;
    Util::writeToCsv(results, "results.csv");
}

int main() {
    try {
        testMethods();
     } catch (const NonSymmetricAndPositiveDefiniteException &ex) {
        std::cout << "Error! The provided matrix is not symmetric and positive definite." << std::endl;
        return 1;
    }
    return 0;
}

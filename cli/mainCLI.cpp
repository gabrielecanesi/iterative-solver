#include <iostream>

#include <Eigen/Sparse>
#include "../runner.h"
#include <util.h>

typedef float precision;


void testMethods() {
    std::string filename = "../Matrices/spa1.mtx";
    std::vector<IterativeBenchmark<precision, Eigen::SparseMatrix<precision>>> results = testMethods<precision>(filename);
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
    testMethods();
    return 0;
}

#include <iostream>

#include <Eigen/Sparse>
#include "../runner.h"
#include <util/util.h>
#include "exceptions/NonSymmetricAndPositiveDefiniteException.h"

typedef double Precision;

struct MatrixFile {
    std::string path;
    std::string name;

    MatrixFile(std::string path, std::string name) : path(path), name(name) {}
};

void testMethods() {
    std::vector<MatrixFile> matrices = {
            MatrixFile("../Matrices/spa1.mtx", "spa1"),
            MatrixFile("../Matrices/spa2.mtx", "spa2"),
            MatrixFile("../Matrices/vem1.mtx", "vem1"),
            MatrixFile("../Matrices/vem2.mtx", "vem2")
    };

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (size_t i = 0; i < matrices.size(); ++i) {
        const auto matrix = matrices[i];

        std::cout << "Matrix: " << matrix.name << std::endl;
        std::cout << "Path: " << matrix.path << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> matrixResults = testMethods<Precision>(matrix.path, false, matrix.name);
        for (auto &method : matrixResults) {
            std::cout << method.methodName() << std::endl;
            std::cout << "\tTolerance: " << method.tolerance() << std::endl;
            std::cout << "\tElapsed: " << method.elapsedMilliseconds() << std::endl;
            std::cout << "\tIterations: " << method.neededIterations() << std::endl;
            std::cout << "\tRelative error: " << method.relativeError() << std::endl << std::endl;
        }

        results.insert(results.end(), matrixResults.begin(), matrixResults.end());

        std::cout << "Matrix non-zero elements: " << MatrixReader::nonZeroProportion(matrix.path) * 100 << "%" << std::endl;
    }

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

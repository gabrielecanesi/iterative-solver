#include <iostream>
#include <map>
#include "IterativeSolver.h"
#include "MatrixReader.h"

#include "ConjugateGradientUpdateStrategy.h"
#include "GradientUpdateStrategy.h"
#include "GaussSeidelUpdateStrategy.h"
#include "JacobiUpdateStrategy.h"
#include "Benchmark.h"
#include <memory>

typedef double precision;

struct MatrixInfo {
    std::string path;
    std::string name;

    MatrixInfo(std::string path, std::string name) : path(path), name(name) {}
};

void testMethods() {
    
    std::vector<precision> testTolerances = {10e-4, 10e-6, 10e-8, 10e-10};
    std::vector<MatrixInfo> matrices = {
            MatrixInfo("./Matrices/spa1.mtx", "spa1"),
            MatrixInfo("./Matrices/spa2.mtx", "spa2"),
            MatrixInfo("./Matrices/vem1.mtx", "vem1"),
            MatrixInfo("./Matrices/vem2.mtx", "vem2")
    };
    std::vector<std::shared_ptr<UpdateStrategy<precision, Eigen::SparseMatrix<precision>>>> methods {
            std::make_shared<GradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
            std::make_shared<ConjugateGradientUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
            std::make_shared<JacobiUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
            std::make_shared<GaussSeidelUpdateStrategy<precision, Eigen::SparseMatrix<precision>>>(),
    };
    IterativeBenchmark<precision, Eigen::SparseMatrix<precision>> benchmark;

    std::map<std::pair<std::string, precision>, BenchmarkResult<precision>> results;

    for (const auto &item : matrices) {
        std::cout << "================= Matrix: " << item.name << " =================" << std::endl;
        Eigen::SparseMatrix<precision> A = MatrixReader::readSparseFromFile<precision>(item.path);
        std::cout << "Non zero proportion: " << MatrixReader::nonZeroProportion(item.path) * 100 << "%" << std::endl;

        Eigen::Matrix<precision, Eigen::Dynamic, 1> x(A.rows(), 1);
        x.setOnes();
        Eigen::Matrix<precision, Eigen::Dynamic, 1> b = A * x;

        for (precision tol : testTolerances) {
            std::cout << "------------- Tolerance: " << tol << " -------------" << std::endl;
            for (auto &strategyPointer: methods) {
                auto result = benchmark.run(A, b, 20000, tol, *strategyPointer.get(), x);
                results[std::make_pair(strategyPointer->name(), tol)] = results[std::make_pair(strategyPointer->name(), tol)] + result;
                std::cout << "---- Method: " << strategyPointer.get()->name() << std::endl;
                std::cout << "\tElapsed: " << benchmark.elapsedMilliseconds() << "ms" << std::endl;
                std::cout << "\tStopped after " << benchmark.neededIterations() << " iterations." << std::endl;
                std::cout << "\tRelative error: " << benchmark.relativeError() << std::endl << std::endl;;
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\n\n\n\n================= Final Results =================" << std::endl;

    for (const auto &result : results) {
        std::cout << "================= Method: " << result.first.first << " =================" << std::endl;
        std::cout << "------------- Tolerance: " << result.first.second << " -------------" << std::endl;
        std::cout << "\tElapsed mean: " << result.second.elapsedMillisecondsMean << "ms" << std::endl;
        std::cout << "\tIteration mean: " << result.second.neededIterationsMean << " iterations." << std::endl;
        std::cout << "\tRelative error: " << result.second.relativeErrorMean << std::endl << std::endl;;
    }

}

int main() {
    testMethods();
    return 0;
}

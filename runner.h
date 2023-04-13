#ifndef MATRIX_RUNNER_H
#define MATRIX_RUNNER_H


#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <solver/IterativeSolver.h>
#include <solver/Solver.h>
#include <util/MatrixReader.h>

#include <updateStrategy/ConjugateGradientUpdateStrategy.h>
#include <updateStrategy/GradientUpdateStrategy.h>
#include <updateStrategy/GaussSeidelUpdateStrategy.h>
#include <updateStrategy/JacobiUpdateStrategy.h>
#include <util/Benchmark.h>
#include <memory>

using namespace UpdateStrategy;

template<typename Precision>
std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> testMethods(const std::string &filename,
                                                                                        Precision tolerance,
                                                                                        bool skipMatrixCheck,
                                                                                        std::string matrixName = "") {

    Eigen::SparseMatrix<Precision> A = MatrixReader::readSparseFromFile<Precision>(filename);
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> b = A * x;

    std::vector<std::shared_ptr<Strategy<Precision, Eigen::SparseMatrix<Precision>>>> methods {
            std::make_shared<GradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
            std::make_shared<ConjugateGradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
            std::make_shared<JacobiUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
            std::make_shared<GaussSeidelUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
    };

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (auto &strategyPointer : methods) {
        auto benchmark = IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>(matrixName);
        benchmark.run(A, b, 20000, tolerance, *strategyPointer.get(), x, skipMatrixCheck);
        results.push_back(benchmark);
    }

    return results;
}


template<typename Precision>
std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> testMethods(const std::string &filename,
                                                                                        bool skipMatrixCheck,
                                                                                        std::string matrixName = "") {

    std::vector<Precision> testTolerances = {10e-4, 10e-6, 10e-8, 10e-10};

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (Precision tol : testTolerances) {
        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> benchMethods = testMethods<Precision>(filename, tol, skipMatrixCheck, matrixName);
        results.insert(results.end(), benchMethods.begin(), benchMethods.end());
    }
    return results;
}

#endif

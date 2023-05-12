#ifndef MATRIX_RUNNER_H
#define MATRIX_RUNNER_H


#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <solver/IterativeSolver.h>
#include <solver/Solver.h>
#include <util/MatrixReader.h>
#include <iostream>

#include <updateStrategy/ConjugateGradientUpdateStrategy.h>
#include <updateStrategy/GradientUpdateStrategy.h>
#include <updateStrategy/GaussSeidelUpdateStrategy.h>
#include <updateStrategy/JacobiUpdateStrategy.h>
#include <util/Benchmark.h>
#include <memory>
#include <solver/NormType.h>

using namespace UpdateStrategy;

template<typename Precision>
std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> testMethods(const std::string &filename,
                                                                                        Precision tolerance,
                                                                                        bool skipMatrixCheck,
                                                                                        Precision gaussW,
                                                                                        Precision jacobiW,
                                                                                        std::string matrixName = "",
                                                                                        NormType normType = NormType::EUCLIDEAN) {

    Eigen::SparseMatrix<Precision> A = MatrixReader::readSparseFromFile<Precision>(filename);
    
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> b = A * x;

    std::vector<std::shared_ptr<Strategy<Precision, Eigen::SparseMatrix<Precision>>>> methods {
            std::make_shared<GradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
            std::make_shared<ConjugateGradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
            std::make_shared<JacobiUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(jacobiW),
            std::make_shared<GaussSeidelUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(gaussW),
    };

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (auto &strategyPointer : methods) {
        auto benchmark = IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>(matrixName);
        benchmark.run(A, b, 20000, tolerance, *strategyPointer.get(), x, skipMatrixCheck, normType);
        results.push_back(benchmark);
    }

    std::cout << results[0].conditionNumber() << std::endl;
    return results;
}


template<typename Precision>
std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> testMethods(const std::string &filename,
                                                                                        bool skipMatrixCheck,
                                                                                        std::string matrixName = "",
                                                                                        Precision gaussW = 1,
                                                                                        Precision jacobiW = 1,
                                                                                        NormType normType = NormType::EUCLIDEAN) {

    std::vector<Precision> testTolerances = {1e-4, 1e-6, 1e-8, 1e-10};

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (Precision tol : testTolerances) {
        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> benchMethods = testMethods<Precision>(filename, tol, skipMatrixCheck, gaussW, jacobiW, matrixName, normType);
        results.insert(results.end(), benchMethods.begin(), benchMethods.end());
    }
    return results;
}

#endif

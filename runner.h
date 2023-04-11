#ifndef MATRIX_RUNNER_H
#define MATRIX_RUNNER_H


#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <IterativeSolver.h>
#include <Solver.h>
#include <MatrixReader.h>

#include <ConjugateGradientUpdateStrategy.h>
#include <GradientUpdateStrategy.h>
#include <GaussSeidelUpdateStrategy.h>
#include <JacobiUpdateStrategy.h>
#include <Benchmark.h>
#include <memory>


template<typename Precision>
std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> testMethods(const std::string &filename) {

    std::vector<Precision> testTolerances = {10e-4, 10e-6, 10e-8, 10e-10};

    Eigen::SparseMatrix<Precision> A = MatrixReader::readSparseFromFile<Precision>(filename);
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> x(A.rows(), 1);
    x.setOnes();
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> b = A * x;

    std::vector<std::shared_ptr<UpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>> methods {
        std::make_shared<GradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
        std::make_shared<ConjugateGradientUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
        std::make_shared<JacobiUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
        std::make_shared<GaussSeidelUpdateStrategy<Precision, Eigen::SparseMatrix<Precision>>>(),
    };

    std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

    for (Precision tol : testTolerances) {
        for (auto &strategyPointer : methods) {
            IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>> benchmark;
            benchmark.run(A, b, 20000, tol, *strategyPointer.get(), x);
            results.push_back(benchmark);
        }

    }
    return results;
}

#endif

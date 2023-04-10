#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <eigen3/Eigen/Dense>
#include "IterativeSolver.h"
#include "UpdateStrategy.h"
#include "timer.h"

#include <iostream>

template<typename Precision, typename MatrixType>
struct BenchmarkResult {

    BenchmarkResult(): elapsedMillisecondsMean(0), neededIterationsMean(0), relativeErrorMean(0), allValues() {}
    BenchmarkResult(unsigned int elapsedMilliseconds, unsigned int neededIterations, Precision relativeError) : elapsedMillisecondsMean(elapsedMilliseconds), neededIterationsMean(neededIterations), relativeErrorMean(relativeError), allValues() {
        allValues.push_back(*this);
    }

    BenchmarkResult& operator+ (const BenchmarkResult &other) {
        allValues.push_back(other);
        update_values();
        return *this;
    }

private:
    void update_values() {
        for (auto &value : allValues) {
            elapsedMillisecondsMean += value.elapsedMillisecondsMean;
            neededIterationsMean += value.neededIterationsMean;
            relativeErrorMean += value.relativeErrorMean;
            ElapsedMillisecondsVariance += value.elapsedMillisecondsMean;
        }
        elapsedMillisecondsMean /= allValues.size();
        neededIterationsMean /= allValues.size();
        relativeErrorMean /= allValues.size();
        ElapsedMillisecondsVariance = std::pow(((ElapsedMillisecondsVariance / allValues.size()) - elapsedMillisecondsMean), 2);
    }

    std::vector<BenchmarkResult<Precision>> allValues;

public:
    unsigned int elapsedMillisecondsMean;
    double ElapsedMillisecondsVariance;
    unsigned int neededIterationsMean;
    Precision relativeErrorMean;

};

template<typename Precision, typename MatrixType>
class IterativeBenchmark {

public:


template<typename Precision, typename MatrixType>
class IterativeBenchmark {
    private:
    Precision M_relativeError;
    Eigen::Matrix<Precision, Eigen::Dynamic, 1> solution;
    IterativeSolver<Precision, MatrixType>* solver;
    Timer timer;

    public:
    IterativeBenchmark() : M_relativeError(0), solver(nullptr) {}
    IterativeBenchmark(const IterativeBenchmark &other) : solver(nullptr) {
        if (other.solver != nullptr) {
            solver = new IterativeSolver<Precision, MatrixType>(*other.solver);
        }
        this->timer = other.timer;
        this->M_relativeError = other.M_relativeError;
        this->solution = other.solution;
    }
    ~IterativeBenchmark() {
        delete solver;
    }

    BenchmarkResult<Precision> run(MatrixType &A, Eigen::Matrix<Precision, Eigen::Dynamic, 1> &b, unsigned int maxIter,
                                   Precision tolerance, UpdateStrategy<Precision, MatrixType> &strategy, const Eigen::Matrix<Precision, Eigen::Dynamic, 1> &x) {

        solver = new IterativeSolver<Precision, MatrixType>(maxIter, &strategy, tolerance);
        timer.tic();
        solution = solver->solve(A, b).eval(); // As a benchmark, we make sure that the whole solution is actually evaluated at this moment.
        timer.toc();
        M_relativeError = (solution - x).squaredNorm() / x.squaredNorm();

        return getBenchmarkResult();
    }

    BenchmarkResult<Precision> getBenchmarkResult() {
        return BenchmarkResult<Precision>(timer.elapsedMilliseconds(), solver->neededIterations(), M_relativeError);
    }

    unsigned int elapsedMilliseconds() {
        return timer.elapsedMilliseconds();
    }

    unsigned int neededIterations() {
        if (solver != nullptr){
            return solver->neededIterations();
        }
        return 0;
    }

    Precision relativeError() {
        return M_relativeError;
    }

    std::string methodName() {
        if (solver == nullptr) {
            return "";
        }
        return solver->methodName();
    }

    Precision tolerance() const {
        if (solver == nullptr) {
            return 0;
        }
        return solver->tolerance();
    }

};

#endif

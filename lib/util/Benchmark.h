#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <Eigen/Dense>
#include "solver/IterativeSolver.h"
#include "updateStrategy/Strategy.h"
#include "util/timer.h"
#include <iostream>

#include <iostream>

template<typename Precision>
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
        elapsedMillisecondsMean = 0;
        neededIterationsMean = 0;
        relativeErrorMean = 0;
        ElapsedMillisecondsVariance = 0;
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
                                   Precision tolerance, UpdateStrategy::Strategy<Precision, MatrixType> &strategy, const Eigen::Matrix<Precision, Eigen::Dynamic, 1> &x) {

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

    unsigned int elapsedMilliseconds() const {
        return timer.elapsedMilliseconds();
    }

    unsigned int neededIterations() const {
        if (solver != nullptr){
            return solver->neededIterations();
        }
        return 0;
    }

    Precision relativeError() const {
        return M_relativeError;
    }

    std::string methodName() const {
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

    std::string toCSVString() const {
        std::stringstream stream;
        stream << this->methodName();
        stream << "," << this->elapsedMilliseconds();
        stream << "," << this->tolerance();
        stream << "," << this->neededIterations();
        stream << "," << this->relativeError();
        return stream.str();
    }

    static std::string CsvHeader() {
        return "method,tolerance,elapsed_ms,iterations,relative_error";
    }
};

template<typename Precision, typename MatrixType>
std::ostream &operator<<(std::ostream& stream, const IterativeBenchmark<Precision, MatrixType> benchmark) {
        stream << benchmark.toCSVString();
        return stream;
}

#endif
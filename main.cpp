#include <iostream>
#include "Solver.h"
#include "IterativeSolver.h"
#include "GradientUpdateStrategy.h"

int main() {
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd b;
    IterativeSolver<double> *solver = new IterativeSolver<double>(50000);
    GradientUpdateStrategy<double> strategy;
    solver->setUpdateStrategy(strategy);
    solver->solve(A, b, 0.0001);
    return 0;

    delete solver;
}
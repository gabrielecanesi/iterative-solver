#include <iostream>

#include "runner.h"

typedef double precision;


void testMethods() {
    std::vector<IterativeBenchmark<double, Eigen::SparseMatrix<double>>> results = testMethods<double>("./Matrices/spa1.mtx");
}

int main() {
    testMethods();
    return 0;
}

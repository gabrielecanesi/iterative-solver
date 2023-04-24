#ifndef ITERATIVE_CLIENT_SOLVE_H
#define ITERATIVE_CLIENT_SOLVE_H

#include "Command.h"
#include "Eigen/Sparse"
#include <util/Benchmark.h>

typedef double Precision;

namespace command {
    class Solve: public Command {
    public:
        Solve(int argc, char *argv[]);
        void parse();
        ~Solve();
        static void printInstruction();

    private:
        std::string path;

    };
}

#endif //ITERATIVE_CLIENT_SOLVE_H

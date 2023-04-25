#ifndef ITERATIVE_CLIENT_COMMAND_H
#define ITERATIVE_CLIENT_COMMAND_H

#include <getopt.h>
#include <iostream>

namespace command {

    class Command {

    public:
        Command(int argc, char *argv[]) : argc(argc), argv(argv), longopts() {};

        virtual ~Command() = default;

        static void printInstruction() {
            std::cout << "Usage: iterative-client <COMMAND> ..." << std::endl;
            std::cout << "Commands:" << std::endl;
            std::cout << "\tsolve <PATH> [OPTION]\t\t\tSolve the linear system for the matrix passed as PATH"
                      << std::endl;
            std::cout << "\ttest [OPTION]\t\t\tExecute all the test on the default matrices" << std::endl;
        }

        static double getDoubleOption(char *optarg) {
            std::string numberToClear = std::string(optarg);
            numberToClear.erase(std::remove(numberToClear.begin(), numberToClear.end(), '='), numberToClear.end());
            numberToClear.erase(std::remove(numberToClear.begin(), numberToClear.end(), ' '), numberToClear.end());
            return std::stod(optarg);
        }

        static std::string getStringOption(char* optarg) {
            std::string numberToClear = std::string(optarg);
            numberToClear.erase(std::remove(numberToClear.begin(), numberToClear.end(), '='), numberToClear.end());
            numberToClear.erase(std::remove(numberToClear.begin(), numberToClear.end(), ' '), numberToClear.end());
            return optarg;
        }

    protected:
        int argc;
        char **argv;
        struct option *longopts;


    };
}

#endif //ITERATIVE_CLIENT_COMMAND_H

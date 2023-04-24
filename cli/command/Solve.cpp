#include "Solve.h"
#include "../runner.h"
#include <iostream>
#include <filesystem>
#include "../exceptions/FileNotFoundException.h"
#include <algorithm>

typedef double Precision;


namespace command {

    Solve::Solve(int argc, char *argv[]): Command(argc, argv) {
        longopts = new struct option [] {
            {"help ", no_argument, nullptr, 'h'},
            {"skipMatrixCheck ", optional_argument, nullptr, 'c'},
            {"jacobiW ", optional_argument, nullptr, 'j'},
            {"gaussW ", optional_argument, nullptr, 'g'},
        };

        if (argc < 2 || argv[2] == nullptr || !std::filesystem::exists(argv[2])) {
            throw FileNotFoundException();
        }

        path = std::string(argv[2]);

    };

    void Solve::printInstruction() {
        std::cout << "Usage: iterative-client Solve <PATH> [OPTIONS]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-h \t--help\t\t\tPrint this instruction menu" << std::endl;
        std::cout << "\t-j \t--jacobiW\t\tPermit to specify the weight for jacobi relaxed method" << std::endl;
        std::cout << "\t-g \t--jacobiW\t\tPermit to specify the weight for Gauss-Seidel relaxed method" << std::endl;
        std::cout << "\t-c \t--skipMatrixCheck\tEnable the checks on the input matrix before execute methods" << std::endl;

    }

    void Solve::parse() {

        bool skipMatrixCheck = false;
        double jacobiW = 1.0;
        double gaussW = 1.0;

        while (true){
            int opt = getopt_long (argc, argv, "hcwg:fF", longopts, nullptr);

            //end of parameter parsing
            if(opt == -1)
                break;

            switch (opt) {
                case 'h':
                    printInstruction();
                    break;
                case 'c':
                    skipMatrixCheck = true;
                    break;
                case 'w':
                    jacobiW = Command::getDoubleOption(optarg);
                    break;
                case 'g':
                    gaussW = Command::getDoubleOption(optarg);
                    break;
            }
        }

        try {
            auto results = testMethods<Precision>(path, skipMatrixCheck, "inputMatrix", gaussW, jacobiW, NormType::EUCLIDEAN);
            for (const auto &result : results){
                std::cout << result.toStdOutput();
            }
        } catch (const NonSymmetricAndPositiveDefiniteException &ex) {
            std::cerr << "Error! The provided matrix is not symmetric and positive definite." << std::endl;
        }

    }

    Solve::~Solve() = default;

}
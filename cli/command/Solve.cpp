#include "Solve.h"
#include "../runner.h"
#include <iostream>
#include <filesystem>
#include "../exceptions/FileNotFoundException.h"
#include <algorithm>

typedef double Precision;


namespace command {

    Solve::Solve(int argc, char *argv[]): Command(argc, argv) {
        longopts = new struct option [5] {
            {"help ", no_argument, nullptr, 'h'},
            {"skipMatrixCheck ", no_argument, nullptr, 'c'},
            {"jacobiW ", required_argument, nullptr, 'j'},
            {"gaussW ", required_argument, nullptr, 'g'},
            {"norm ", required_argument, nullptr, 'n'}
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
        std::cout << "\t-n \t--norm\t\t\tSelect the norm type when coputing the convergence (euclidean, manhattan, infinity)" << std::endl;

    }

    void Solve::parse() {
        bool skipMatrixCheck = false;
        double jacobiW = 1.0;
        double gaussW = 1.0;
        NormType norm = NormType::EUCLIDEAN;
        int opt;
        
        while ((opt = getopt_long (argc, argv, "n: hc j:fF g:fF", longopts, nullptr)) != -1){
            switch (opt) {
                    case 'h':
                        printInstruction();
                        break;
                    case 'c':
                        skipMatrixCheck = true;
                        break;
                    case 'j':
                        jacobiW = Command::getDoubleOption(optarg);
                        break;
                    case 'g':
                        gaussW = Command::getDoubleOption(optarg);
                        break;
                    case 'n':
                        std::string normString = Command::getStringOption(optarg);
                        if (normString == "euclidean") {
                            norm = NormType::EUCLIDEAN;
                        } else if (normString == "manhattan") {
                            norm = NormType::MANHATTAN;
                        } else if (normString == "infinity") {
                            norm = NormType::INFTY;
                        }
                }
        }
        

        try {
            auto results = testMethods<Precision>(path, skipMatrixCheck, "inputMatrix", gaussW, jacobiW, norm);
            for (const auto &result : results){
                std::cout << result.toStdOutput();
            }
        } catch (const NonSymmetricAndPositiveDefiniteException &ex) {
            std::cerr << "Error! The provided matrix is not symmetric and positive definite." << std::endl;
        } catch (const WrongParameterValueException &ex) {
            std::cerr << "Error! The passed parameter is invalid, one of the methods may not converge" << std::endl;
        } catch (const NonSquareMatrixException &ex) {
            std::cerr << "Error! The provided matrix is not square" << std::endl;
        }

    }

    Solve::~Solve() = default;

}
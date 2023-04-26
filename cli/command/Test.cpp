#include "Test.h"
#include "../runner.h"
#include <util/util.h>

typedef double Precision;

namespace command {

    Test::Test(int argc, char **argv): Command(argc, argv) {
        longopts = new struct option [5] {
            {"help", no_argument, nullptr, 'h'},
            {"skipMatrixCheck ", no_argument, nullptr, 'c'},
            {"jacobiW ", required_argument, nullptr, 'j'},
            {"gaussW ", required_argument, nullptr, 'g'},
            {"norm ", required_argument, nullptr, 'n'}
        };

        ++argv;
    }

    void Test::printInstruction() {
        std::cout << "Usage: iterative-client test [OPTIONS] ..." << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-h \t--help\t\t\tPrint this instruction menu" << std::endl;
        std::cout << "\t-j \t--jacobiW\t\tPermit to specify the weight for jacobi relaxed method" << std::endl;
        std::cout << "\t-g \t--jacobiW\t\tPermit to specify the weight for Gauss-Seidel relaxed method" << std::endl;
        std::cout << "\t-c \t--skipMatrixCheck\tEnable the checks on the input matrix before execute methods" << std::endl;
        std::cout << "\t-n \t--norm\t\t\tSelect the norm type when coputing the convergence (euclidean, manhattan, infinity)" << std::endl;
    }

    void test(std::string path, std::string matrix_name, std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> *results, bool skipMatrixCheck, double gaussW, double jacobiW, NormType norm){

        std::cout << "Matrix: " << matrix_name << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> matrixResults = testMethods<Precision>(path, skipMatrixCheck, "inputMatrix", gaussW, jacobiW, norm);
        for (const auto &result : matrixResults){
            std::cout << result.toStdOutput();
        }

        if(results != nullptr)
            results->insert(results->end(), matrixResults.begin(), matrixResults.end());
    }

    void test(bool skipMatrixCheck, double gaussW, double jacobiW, NormType norm) {
        std::vector<MatrixFile> matrices = {
                MatrixFile("../Matrices/spa1.mtx", "spa1"),
                MatrixFile("../Matrices/spa2.mtx", "spa2"),
                MatrixFile("../Matrices/vem1.mtx", "vem1"),
                MatrixFile("../Matrices/vem2.mtx", "vem2")
        };

        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

        for (size_t i = 0; i < matrices.size(); ++i) {
            const auto matrix = matrices[i];

            test(matrix.path, matrix.name, &results, skipMatrixCheck, gaussW, jacobiW, norm);
        }

        Util::writeToCsv(results, "results.csv");
    }

    void Test::parse() {
        bool skipMatrixCheck = false;
        double jacobiW = 1.0;
        double gaussW = 1.0;
        NormType norm = NormType::EUCLIDEAN;
        int opt;
        std::cout << (int)norm << std::endl;
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
            test(skipMatrixCheck, gaussW, jacobiW, norm);
        } catch (const NonSymmetricAndPositiveDefiniteException &ex) {
            std::cerr << "Error! The provided matrix is not symmetric and positive definite." << std::endl;
        } catch (const WrongParameterValueException &ex) {
            std::cerr << "Error! The passed parameter is invalid, one of the methods may not converge" << std::endl;
        } catch (const NonSquareMatrixException &ex) {
            std::cerr << "Error! The provided matrix is not square" << std::endl;
        }
    }
}
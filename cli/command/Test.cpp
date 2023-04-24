#include "Test.h"
#include "../runner.h"
#include <util/util.h>

typedef double Precision;

namespace command {

    Test::Test(int argc, char **argv): Command(argc, argv) {
        longopts = new struct option [] {
            {"help", no_argument, nullptr, 'h'},
            {"skipMatrixCheck ", optional_argument, nullptr, 'c'},
            {"jacobiW ", optional_argument, nullptr, 'j'},
            {"gaussW ", optional_argument, nullptr, 'g'}
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
    }

    void test(std::string path, std::string matrix_name, std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> *results, bool skipMatrixCheck, double gaussW, double jacobiW){

        std::cout << "Matrix: " << matrix_name << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> matrixResults = testMethods<Precision>(path, skipMatrixCheck, "inputMatrix", gaussW, jacobiW, NormType::EUCLIDEAN);
        for (const auto &result : matrixResults){
            std::cout << result.toStdOutput();
        }

        if(results != nullptr)
            results->insert(results->end(), matrixResults.begin(), matrixResults.end());
    }

    void test(bool skipMatrixCheck, double gaussW, double jacobiW) {
        std::vector<MatrixFile> matrices = {
                MatrixFile("../Matrices/spa1.mtx", "spa1"),
                MatrixFile("../Matrices/spa2.mtx", "spa2"),
                MatrixFile("../Matrices/vem1.mtx", "vem1"),
                MatrixFile("../Matrices/vem2.mtx", "vem2")
        };

        std::vector<IterativeBenchmark<Precision, Eigen::SparseMatrix<Precision>>> results;

        for (size_t i = 0; i < matrices.size(); ++i) {
            const auto matrix = matrices[i];

            test(matrix.path, matrix.name, &results, skipMatrixCheck, gaussW, jacobiW);
        }

        Util::writeToCsv(results, "results.csv");
    }

    void Test::parse() {
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
            test(skipMatrixCheck, gaussW, jacobiW);
        } catch (const NonSymmetricAndPositiveDefiniteException &ex) {
            std::cerr << "Error! The provided matrix is not symmetric and positive definite." << std::endl;
        }
    }
}
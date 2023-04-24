#include <iostream>

#include "exceptions/NonSymmetricAndPositiveDefiniteException.h"
#include "command/Solve.h"
#include "command/Test.h"
#include "command/Command.h"
#include "exceptions/FileNotFoundException.h"

typedef double Precision;

struct MatrixFile {
    std::string path;
    std::string name;

    MatrixFile(std::string path, std::string name) : path(path), name(name) {}
};

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        std::cerr << "Error! No command provided." << std::endl;
        command::Command::printInstruction();
        return -1;
    }
    std::string input_command = argv[1];

    if(input_command == "solve"){
        try {
            auto solverCommand = command::Solve(argc, argv);
            solverCommand.parse();
        } catch (const FileNotFoundException &ex) {
            std::cerr << "File not found" << std::endl;
            command::Solve::printInstruction();
            return -1;
        }

    } else if (input_command == "test") {
        auto testCommand = command::Test(argc, argv);
        testCommand.parse();
    }

    return 0;
}

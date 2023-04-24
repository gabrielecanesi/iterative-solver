#ifndef ITERATIVE_CLIENT_TEST_H
#define ITERATIVE_CLIENT_TEST_H

#include "Command.h"

namespace command {

    class Test : public Command {
    public:
        Test(int argc, char **argv);
        void printInstruction();
        void parse();


    };

    struct MatrixFile {
        std::string path;
        std::string name;

        MatrixFile(std::string path, std::string name) : path(path), name(name) {}
    };
}



#endif //ITERATIVE_CLIENT_TEST_H

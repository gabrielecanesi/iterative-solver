#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <fstream>

namespace Util {
    template<typename T>
    void writeToCsv(const std::vector<T> &elements, const std::string &filename) {
        std::ofstream outStream(filename);
        outStream << T::CsvHeader() << std::endl;
        for (auto &element : elements) {
            outStream << element << std::endl;
        }
    }
}


#endif
#include "MatrixReader.h"
#include <fstream>

double MatrixReader::nonZeroProportion(const std::string &path) {
    std::ifstream infile(path);
    double rows, columns, nonZero;
    infile >> rows >> columns >> nonZero;
    return (nonZero / columns) / rows; // Avoid overflows by rows and columns multiplication
}

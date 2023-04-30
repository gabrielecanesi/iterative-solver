#include "MatrixReader.h"
#include <fstream>
#include <iostream>

double MatrixReader::nonZeroProportion(const std::string &path) {
    std::ifstream infile(path);
    double rows, columns, nonZero;
    infile >> rows >> columns >> nonZero;
    return (nonZero / columns) / rows; // Avoid overflows by rows and columns multiplication
}

void MatrixReader::clearMatrix(std::ifstream* inf){

    while(inf->peek() == 37){
        inf->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
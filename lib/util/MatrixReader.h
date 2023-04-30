#ifndef SPARSE_READER_H
#define SPARSE_READER_H

#include <fstream>
#include <Eigen/Sparse>
#include <iostream>

namespace MatrixReader {

    void clearMatrix(std::ifstream* inf);

    template<typename T>
    Eigen::SparseMatrix<T> readSparseFromFile(const std::string &path) {
        std::vector<Eigen::Triplet<T>> entries;
        std::ifstream infile(path);
        unsigned int rows, columns, nonZero;
        clearMatrix(&infile);
        infile >> rows >> columns >> nonZero;
        Eigen::SparseMatrix<T> result(rows, columns);
        //std::cout << rows + "," + columns + ", " + nonZero;
        unsigned int row, column;
        T value;
        while (infile >> row >> column >> value){
            entries.push_back(Eigen::Triplet<T>(row - 1, column - 1, value));
        }

        result.setFromTriplets(entries.begin(), entries.end());
        return result;
    }

    double nonZeroProportion(const std::string &path);

}
    
#endif
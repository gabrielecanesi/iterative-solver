#ifndef SPARSE_READER_H
#define SPARSE_READER_H

#include <fstream>
#include <eigen3/Eigen/Sparse>

namespace MatrixReader {

    template<typename T>
    Eigen::SparseMatrix<T> readSparseFromFile(const std::string &path) {
        std::vector<Eigen::Triplet<T>> entries;
        std::ifstream infile(path);
        unsigned int rows, columns, nonZero;
        infile >> rows >> columns >> nonZero;
        Eigen::SparseMatrix<T> result(rows, columns);
        unsigned int row, column;
        T value;
        while (infile >> row >> column >> value){
            entries.push_back(Eigen::Triplet<T>(row - 1, column - 1, value));
        }

        result.setFromTriplets(entries.begin(), entries.end());
        return result;
    }

    double nonZeroProportion(const std::string &path) {
        std::ifstream infile(path);
        double rows, columns, nonZero;
        infile >> rows >> columns >> nonZero;
        return (nonZero / columns) / rows; // Avoid overflows by rows and columns multiplication
    }
}

#endif
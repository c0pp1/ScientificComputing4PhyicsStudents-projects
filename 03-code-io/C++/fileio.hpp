#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <iostream>
#include <fstream>
#include <string>

void read_vector_binary(int N, const std::string &fname, double * &vector) {
    std::ifstream file(fname, std::ios::binary);
    if (!file) {
        std::cerr << "Error: cannot open file <" << fname << ">\n";
        std::exit(EXIT_FAILURE);
    }

    if (vector == nullptr)
        vector = new double[N];
        if (vector == nullptr) {
            std::cerr << "Memory allocation failed\n";
            std::exit(EXIT_FAILURE);
        }

    file.read(reinterpret_cast<char*>(vector), N * sizeof(double));
    if (!file) {
        std::cerr << "Error: failed to read data from file <" << fname << ">\n";
        delete[] vector;
        std::exit(EXIT_FAILURE);
    }
}

void dump_vector_binary(int N, const std::string &fname, double *vect) {
    std::ofstream file(fname, std::ios::binary);
    if (!file) {
        std::cerr << "Error: cannot open file <" << fname << ">\n";
        delete[] vect;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Writing file <" << fname << "> ...";
    file.write(reinterpret_cast<const char*>(vect), N * sizeof(double));
    if (!file) {
        std::cerr << "Error: failed to write data to file <" << fname << ">\n";
        delete[] vect;
        std::exit(EXIT_FAILURE);
    }
    std::cout << " done.\n";
}

#endif //FILEIO_HPP
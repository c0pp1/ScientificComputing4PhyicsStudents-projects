#include <iostream>
#include <random>
#include <string>

#include "fileio.hpp"
#include "parser.h"

using namespace std;

void generate_vector(int N, double * &v, double mean, double std) {
    // allocate memory for vector if not already allocated
    if (v == nullptr) {
        v = new double[N];
        if (v == nullptr) {
            std::cerr << "Memory allocation failed\n";
            exit(EXIT_FAILURE);
        }
    }

    // initialize random number generator
    default_random_engine generator;
    normal_distribution<double> d(mean, std);

    // else we assume the memory is already allocated 
    // and initialize vector
    for (int i = 0; i < N; i++) *(v + i) = d(generator);
}

int main(int argv, char *argc[]) {

    std::cout << "Hi there! " << argc[0] << " is running..." << std::endl;

    string fname_config = "daxpy.conf";
    string of_prefix = "";
    size_t N = 0;
    double mean = 0.0, std = 1.0;

    // Read config from file
    config_option_t co;
    if ((co = read_config_file(fname_config.c_str())) == NULL) {
        printf("read_config_file() error");
        return EXIT_FAILURE;
    }
    printf("parsed from .conf:\n");
    while(1) {
        cout << "\t- " << co->key << " -> " << co->value << endl;

        if (strcmp(co->key, "N") == 0) {
            N = atoi(co->value);
        } else if (strcmp(co->key, "if_prefix") == 0) {
            of_prefix = co->value;
        } else if (strcmp(co->key, "mean") == 0) {
            mean = atof(co->value);
        } else if (strcmp(co->key, "std") == 0) {
            std = atof(co->value);
        }
        
        if (co->prev != NULL) co = co->prev;
        else break;
    }
    cout << "Generating vectors with N=" << N << " elements on files " << of_prefix << endl;

    double *vec = nullptr;
    cout << "Generating vector X ..." << endl;
    generate_vector(N, vec, mean, std);

    string filename = of_prefix + "_N" + to_string(N) + "_x.dat";
    dump_vector_binary(N, filename, vec);

    cout << "Generating vector Y ...\n";
    generate_vector(N, vec, mean, std);

    filename = of_prefix + "_N" + to_string(N) + "_y.dat";
    dump_vector_binary(N, filename, vec);
    
    // clean up
    delete[] vec;
    return 0;

}
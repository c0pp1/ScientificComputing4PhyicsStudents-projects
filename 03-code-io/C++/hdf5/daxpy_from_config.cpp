#include <iostream>
#include <string>

#include "fileio.hpp"
#include "../parser.h"

using namespace std;
void daxpy(int n, double a, double *x, double *y) {

    if (n <= 0 || a == 0.0) {
        return;
    }

    int m = n % 4;
    for (int i = 0; i < m; i++) {
        y[i] += a * x[i];
    }

    for (int i = m; i < n; i += 4) {
        y[i] += a * x[i];
        y[i + 1] += a * x[i + 1];
        y[i + 2] += a * x[i + 2];
        y[i + 3] += a * x[i + 3];
    }
}

int main(int argc, char* argv[]) {

    string fname_config = "daxpy.conf";
    string fname_x = "";
    string fname_y = "";
    string fname_prefix = "";
    size_t N = 0;
    double a = 0.0;

    // Read config from file
    config_option_t co;
    if ((co = read_config_file(fname_config.c_str())) == NULL) {
        cout << "read_config_file() error" << endl;
        return EXIT_FAILURE;
    }
    cout << "parsed from .conf:" << endl;
    while(1) {
        cout << "\t- " << co->key << " -> " << co->value << endl;

        if (strcmp(co->key, "fname_x") == 0) {
            fname_x = co->value;
        } else if (strcmp(co->key, "fname_y") == 0) {
            fname_y = co->value;
        } else if (strcmp(co->key, "N") == 0) {
            N = atoi(co->value);
        } else if (strcmp(co->key, "a") == 0) {
            a = atof(co->value);
        } else if (strcmp(co->key, "prefix") == 0) {
            fname_prefix = co->value;
        }
        
        if (co->prev != NULL) co = co->prev;
        else break;
    }

    // Allocate memory on the heap
    double* x = nullptr, *y = nullptr;

    // Initialize x and y
    // reading x and y
    read_vector_hdf5(N, fname_x, "vector_data", x);
    read_vector_hdf5(N, fname_y, "vector_data", y);

    daxpy(N, a, x, y);

    cout << "[info] look at first result: d[0] = " << y[0] << endl;
    dump_vector_hdf5(N, fname_prefix + "_N" + to_string(N) + "_d.h5", "vector_data", y);
    read_vector_hdf5(N, fname_prefix + "_N" + to_string(N) + "_d.h5", "vector_data", x);
    cout << "[info] look at first read result: d[0] = " << x[0] << endl;

    delete[] x;
    delete[] y;

    return 0;
}
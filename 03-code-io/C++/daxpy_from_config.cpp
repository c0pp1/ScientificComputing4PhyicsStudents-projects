#include <iostream>
#include <string>

#include "../C/fileio.h"
#include "../C/parser.h"

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
        printf("read_config_file() error");
        return EXIT_FAILURE;
    }
    printf("parsed from .conf:\n");
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
    double *x = new double[N];
    double *y = new double[N];

    // Check if memory allocation was successful
    if (x == nullptr || y == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }

    // Initialize x and y
    // reading x and y
    read_vector_binary(N, fname_x.c_str(), &x);
    read_vector_binary(N, fname_y.c_str(), &y);

    daxpy(N, a, x, y);

    dump_vector_binary(N, (fname_prefix + "_N" + to_string(N) + "_d.dat").c_str(), y);

    delete[] x;
    delete[] y;

    return 0;
}
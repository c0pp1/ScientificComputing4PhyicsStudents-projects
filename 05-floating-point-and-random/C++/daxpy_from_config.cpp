#include <iostream>
#include <cmath>
#include <string>

#include "fileio.hpp"
#include "parser.h"

using namespace std;

double KahanBabushkaNeumaierSum(const double *vec, int n) {
    /*
    Kahan-Babushka-Neumaier summation algorithm
    This algorithm is a modification of the Kahan summation algorithm that uses two variables
    to keep track of the compensation for lost low-order bits.
    */

    double sum = 0.0, c = 0.0;

    for (int i = 0; i < n; i++) {
        double t = sum + vec[i];
        if (abs(sum) >= abs(vec[i])) {
            c += (sum - t) + vec[i]; // c is the compensation for low-order bits lost from vec[i]
        } else {
            c += (vec[i] - t) + sum; // c is the compensation for low-order bits lost from sum
        }
        sum = t;
    }
    return sum + c;
}

double stdev(const double *vec, int n) {
    /*
    Standard deviation calculation
    This function calculates the standard deviation of a vector using the Kahan-Babushka-Neumaier summation algorithm.
    */

    double mean = KahanBabushkaNeumaierSum(vec, n) / n;
    double sum_sq_diff = 0.0;

    for (int i = 0; i < n; i++) {
        sum_sq_diff += pow(vec[i] - mean, 2);
    }

    return sqrt(sum_sq_diff / n-1);
}

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
    string if_prefix = "", of_prefix = "";
    size_t N = 0;
    double a = 0.0, mean = 0.0, std = 1.0;

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
        } else if (strcmp(co->key, "a") == 0) {
            a = atof(co->value);
        } else if (strcmp(co->key, "of_prefix") == 0) {
            of_prefix = co->value;
        } else if (strcmp(co->key, "if_prefix") == 0) {
            if_prefix = co->value;
        } else if (strcmp(co->key, "mean") == 0) {
            mean = atof(co->value);
        } else if (strcmp(co->key, "std") == 0) {
            std = atof(co->value);
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
    string fname_x = if_prefix + "_N" + to_string(N) + "_x.dat";
    string fname_y = if_prefix + "_N" + to_string(N) + "_y.dat";
    read_vector_binary(N, fname_x, x);
    read_vector_binary(N, fname_y, y);

    daxpy(N, a, x, y);

    string ofname = of_prefix + "_N" + to_string(N) + "_d.dat";
    dump_vector_binary(N, ofname, y);

    // Check the result
    double exp_mean = KahanBabushkaNeumaierSum(y, N) / N;
    double exp_std = stdev(y, N);
    double th_std = std * sqrt(a*a + 1.);

    int tol = 1; // tolerance for the mean in terms of sigmas
    if(abs(exp_mean - mean) > tol * th_std) cout << "[warning] mean is not as expected!" << endl;
    else cout << "[info] Mean is as expected!" << endl;

    cout << "Experimental mean +- std:\t" << exp_mean << " +- " << exp_std << endl;
    cout << "Expected mean +- std:\t\t" << mean << " +- " << th_std << endl;


    delete[] x;
    delete[] y;

    return 0;
}
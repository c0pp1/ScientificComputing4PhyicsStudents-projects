#include <cmath>
#include <chrono>
#include <iostream>

void square_matmul(size_t n, double *a, double *b, double *c) {
    // This function computes the matrix product C = A * B
    // where A, B, and C are n x n matrices.
    // Assumes the matrices are stored in row-major order.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

bool allclose(double *C, double expected, size_t &n, double tolerance) {
    // This function checks if all elements of C are close to the expected value
    // within the specified tolerance.
    // It returns true if all elements are close, false otherwise.
    // It also updates n to the index of the first false check.

    for (int i = 0; i < n * n; i++) {
        if (fabs(C[i] - expected) >= tolerance) {
            n = i;
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    const double TOLERANCE = 1e-9;
    const size_t N[] = {10, 100, 1000, 10000};
    const double a = 3.;
    const double b = 7.1;

    for (const size_t n: N) {
        std::cout << "Testing for N = " << n << "..." << std::endl;

        // Allocate memory on the heap since it will
        // scale quickly with n*n.
        double *A = new double[n * n];
        double *B = new double[n * n];
        double *C = new double[n * n];

        if (A == nullptr || B == nullptr || C == nullptr) {
            std::cerr << "[error] memory allocation failed" << std::endl;
            return 1;
        }

        for (size_t i = 0; i < n * n; i++) {
            A[i] = a;
            B[i] = b;
        }
        
        // Measure the time taken for the matrix multiplication
        auto start = std::chrono::high_resolution_clock::now();
        square_matmul(n, A, B, C);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken for matrix multiplication: " << elapsed.count() << " seconds" << std::endl;

        std::cout << "Checking the result..." << std::endl;
        size_t first_false = n;
        double expected_value = a * b * n;
        bool test = allclose(C, expected_value, first_false, TOLERANCE);
        if (test) {
            std::cout << "Passed" << std::endl;
        } else {
            std::cout << "Failed" << std::endl;
            printf("C[%d] = %.15f but expected %.15f\n", first_false, C[first_false], expected_value);
            delete[] A;
            delete[] B;
            delete[] C;
            return 1;
        }

        delete[] A;
        delete[] B;
        delete[] C;
    }
    return 0;
}
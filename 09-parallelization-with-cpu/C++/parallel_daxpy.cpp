#include <assert.h>
#include <cmath>
#include <iostream>
#include <chrono>

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

double KahanBabushkaNeumaierSum_parallel(const double *vec, int n) {
    /*
    Kahan-Babushka-Neumaier summation algorithm
    This algorithm is a modification of the Kahan summation algorithm that uses two variables
    to keep track of the compensation for lost low-order bits.
    */

    double sum = 0.0, c = 0.0;

    #pragma omp parallel for reduction(+:sum,c)
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

void daxpy_chunked(int n, double a, double *x, double *y, int chunk_size=0) {

    if (n <= 0 || a == 0.0) {
        return;
    }

    if (chunk_size <= 1) {
        chunk_size = n; // default chunk size
    }
    assert(chunk_size <= n);

    int remainder = n % chunk_size;
    // Process eventual smaller chunk first
    for (int i = 0; i < remainder; i++) {
        y[i] += a * x[i];
    }

    for (int chunk_start = remainder; chunk_start < n; chunk_start += chunk_size) {
        for (int i = 0; i < chunk_size; i++) {
            y[chunk_start + i] += a * x[chunk_start + i];
        }
    }
}

void daxpy_chunked_parallel(int n, double a, double *x, double *y, int chunk_size=0) {

    if (n <= 0 || a == 0.0) {
        return;
    }

    if (chunk_size <= 1) {
        chunk_size = n; // default chunk size
    }
    assert(chunk_size <= n);

    int remainder = n % chunk_size;
    // Process eventual smaller chunk first
    for (int i = 0; i < remainder; i++) {
        y[i] += a * x[i];
    }

    #pragma omp parallel for
    for (int chunk_start = remainder; chunk_start < n; chunk_start += chunk_size) {
        for (int i = 0; i < chunk_size; i++) {
            y[chunk_start + i] += a * x[chunk_start + i];
        }
    }
}

void daxpy_parallel(int n, double a, double *x, double *y) {

    if (n <= 0 || a == 0.0) {
        return;
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        y[i] += a * x[i];
    }

}

double sum_chunked(int n, double *x, int chunk_size=0) {

    if (n <= 0) {
        return 0.0;
    }

    if (chunk_size <= 1) {
        chunk_size = n; // default chunk size
    }
    assert(chunk_size <= n);

    int n_chunks = n / chunk_size;
    int remainder = n % chunk_size;
    double* partial_sums = new double[n_chunks + (remainder > 0 ? 1 : 0)];

    // Process eventual smaller chunk first
    // but put it in last element of partial_sums array
    double sum = 0.0;
    for (int i = 0; i < remainder; i++) {
        sum += x[i];
    }
    partial_sums[n_chunks - (remainder > 0 ? 0 : 1)] = sum;

    // Now process all full chunks;
    for (int chunk = 0, start_index = remainder; chunk < n_chunks; chunk++, start_index += chunk_size) {
        partial_sums[chunk] = KahanBabushkaNeumaierSum(x + start_index, chunk_size);
    }

    // Now sum up all partial sums
    return KahanBabushkaNeumaierSum(partial_sums, n_chunks + (remainder > 0 ? 1 : 0));
}

double sum_chunked_parallel(int n, double *x, int chunk_size=0) {

    if (n <= 0) {
        return 0.0;
    }

    if (chunk_size <= 1) {
        chunk_size = n; // default chunk size
    }
    assert(chunk_size <= n);

    int n_chunks = n / chunk_size;
    int remainder = n % chunk_size;
    double* partial_sums = new double[n_chunks + (remainder > 0 ? 1 : 0)];

    // Process eventual smaller chunk first
    // but put it in last element of partial_sums array
    double sum = 0.0;
    for (int i = 0; i < remainder; i++) {
        sum += x[i];
    }
    partial_sums[n_chunks - (remainder > 0 ? 0 : 1)] = sum;

    // Now process all full chunks;
    #pragma omp parallel for
    for (int chunk = 0; chunk < n_chunks; chunk++) {
        int start_index = remainder + chunk * chunk_size;
        partial_sums[chunk] = KahanBabushkaNeumaierSum(x + start_index, chunk_size);
    }

    // Now sum up all partial sums
    return KahanBabushkaNeumaierSum(partial_sums, n_chunks + (remainder > 0 ? 1 : 0));
}


int main(int argc, char* argv[]) {
    
    const double TOLERANCE = 1e-10;
    const double a = 3.;
    const size_t ARRAY_SIZES[] = {10, 1000, 10000, 1000000, 100000000};
    const size_t CHUNK_SIZES[] = {1, 4, 8, 10};

    // Test memory allocation on the stack and heap
    // for each array size and implementation of daxpy
    for (const size_t n: ARRAY_SIZES) {

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Testing with n = " << n << std::endl;

        // Allocate memory on the heap
        double *x = new double[n];
        double *y = new double[n];

        // Check if memory allocation was successful
        if (x == nullptr || y == nullptr) {
            std::cerr << "Memory allocation failed" << std::endl;
            return 1;
        }
        // Initialize x
        for (size_t j = 0; j < n; j++) {
            x[j] = 0.1;
        }

        for (const size_t chunk_size: CHUNK_SIZES) {
 
            // Since the y array will be modified by the previous daxpy call,
            // have to reinitialize it.
            for (size_t j = 0; j < n; j++) {
                y[j] = 7.1;
            }

            // chunked implementation
            auto start = std::chrono::high_resolution_clock::now();
            daxpy_chunked(n, a, x, y, chunk_size);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "chunk size " << chunk_size << "\n\t daxpy time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result
            for (size_t j = 0; j < n; j++) {
                assert(fabs(y[j] - (7.1 + a * 0.1)) < TOLERANCE);
            }

            // sum
            start = std::chrono::high_resolution_clock::now();
            auto sum = sum_chunked(n, y, chunk_size);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "\t sum time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result
            assert(fabs(sum - n * (7.1 + a * 0.1)) < n*TOLERANCE);

            // chunked parallel implementation
            // Since the y array will be modified by the previous daxpy call,
            // have to reinitialize it.
            for (size_t j = 0; j < n; j++) {
                y[j] = 7.1;
            }
            start = std::chrono::high_resolution_clock::now();
            daxpy_chunked_parallel(n, a, x, y, chunk_size);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "\t daxpy chunked parallel time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result
            for (size_t j = 0; j < n; j++) {
                assert(fabs(y[j] - (7.1 + a * 0.1)) < TOLERANCE);
            }
            // sum chunked parallel
            start = std::chrono::high_resolution_clock::now();
            sum = sum_chunked_parallel(n, y, chunk_size);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "\t sum chunked parallel time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result
            assert(fabs(sum - n * (7.1 + a * 0.1)) < n*TOLERANCE);

        }
        // parallel implementation
        // Since the y array will be modified by the previous daxpy call,
        // have to reinitialize it.
        for (size_t j = 0; j < n; j++) {
            y[j] = 7.1;
        }
        auto start = std::chrono::high_resolution_clock::now();
        daxpy_parallel(n, a, x, y);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "parallel daxpy time: " << elapsed.count() << " seconds" << std::endl;
        // Verify result
        for (size_t j = 0; j < n; j++) {
            assert(fabs(y[j] - (7.1 + a * 0.1)) < TOLERANCE);
        }
        // sum parallel
        start = std::chrono::high_resolution_clock::now();
        auto sum = KahanBabushkaNeumaierSum_parallel(y, n);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "parallel sum time: " << elapsed.count() << " seconds" << std::endl;
        // Verify result
        assert(fabs(sum - n * (7.1 + a * 0.1)) < n*TOLERANCE);

        delete[] x;
        delete[] y;
        
        std::cout << "----------------------------------------" << std::endl;
    }
    return 0;
}
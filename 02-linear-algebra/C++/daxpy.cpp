#include <assert.h>
#include <cmath>
#include <iostream>
#include <chrono>


void daxpy(int n, double a, double *x, double *y) {
    if (n <= 0 || a == 0.0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        y[i] += a * x[i];
    }
}

void daxpy_unrolled(int n, double a, double *x, double *y) {

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
    
    const double TOLERANCE = 1e-15;
    const double a = 3.;
    const size_t ARRAY_SIZES[] = {10, 1000, 10000, 1000000, 100000000};

    // Test memory allocation on the stack and heap
    // for each array size and implementation of daxpy
    for (const size_t n: ARRAY_SIZES) {

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Testing with n = " << n << std::endl;

        // Allocate memory on the heap
        auto start_heap = std::chrono::high_resolution_clock::now();
        double *x = new double[n];
        double *y = new double[n];
        auto end_heap = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_heap = end_heap - start_heap;
        std::cout << "Heap allocation time: " << elapsed_heap.count() << " seconds" << std::endl;

        // Check if memory allocation was successful
        if (x == nullptr || y == nullptr) {
            std::cerr << "Memory allocation failed" << std::endl;
            return 1;
        }

        // Initialize x and y
        auto start_init = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < n; j++) {
            x[j] = 0.1;
            y[j] = 7.1;
        }
        auto end_init = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_init = end_init - start_init;
        std::cout << "Heap initialization time: " << elapsed_init.count() << " seconds" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        daxpy(n, a, x, y);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "daxpy_heap: " << elapsed.count() << " seconds" << std::endl;

        // The y array now contains the expected result of the daxpy operation
        // We assert that the result is correct in a tolerance range
        for (size_t j = 0; j < n; j++) {
            assert(fabs(y[j] - (7.1 + a * 0.1)) < TOLERANCE);
        }

        start = std::chrono::high_resolution_clock::now();
        daxpy_unrolled(n, a, x, y);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "daxpy_unrolled_heap: " << elapsed.count() << " seconds" << std::endl;
        // Since the y array was modified by the previous daxpy call,
        // the result will not be the same as the first call.
        // result of first call: y[j] = y[j] + a * 0.1 = 7.1 + 3. * 0.1 = 7.4
        // result of second call: y[j] = y[j] + a * 0.1 = 7.4 + 3. * 0.1 = 7.7
        // Beaware of pass by reference/pointer!


        delete[] x;
        delete[] y;

        // Allocate memory on the stack
        // Note: Stack allocation is limited by the stack size, which is usually much smaller than heap allocation.
        // In practice, you would not allocate large arrays on the stack.
        // Here we check if the size of the array exceeds a certain limit (e.g., 7.5 MB)
        if (n * sizeof(double) > 7.5 * 1024 * 1024) {
            std::cerr << "Stack allocation size exceeds limit" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            continue;
        }

        auto start_stack = std::chrono::high_resolution_clock::now();
        double x_stack[n];
        double y_stack[n];
        auto end_stack = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_stack = end_stack - start_stack;
        std::cout << "Stack allocation time: " << elapsed_stack.count() << " seconds" << std::endl;

        // Check if memory allocation was successful
        if (x_stack == nullptr || y_stack == nullptr) {
            std::cerr << "Memory allocation failed" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            return 1;
        }

        // Initialize x and y
        auto start_init_stack = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < n; j++) {
            x_stack[j] = 0.1;
            y_stack[j] = 7.1;
        }
        auto end_init_stack = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_init_stack = end_init_stack - start_init_stack;
        std::cout << "Stack initialization time: " << elapsed_init_stack.count() << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        daxpy(n, a, x_stack, y_stack);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "daxpy_stack: " << elapsed.count() << " seconds" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        daxpy_unrolled(n, a, x_stack, y_stack);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "daxpy_unrolled_stack: " << elapsed.count() << " seconds" << std::endl;
        
        std::cout << "----------------------------------------" << std::endl;
    }
    return 0;
}
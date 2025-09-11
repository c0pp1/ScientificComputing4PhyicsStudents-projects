#include <assert.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <mpi.h>

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

void daxpy_chunked_parallel(int n, double a, double *x, double *y) {

    int world_size, this_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &this_rank);

    if (n <= 0 || a == 0.0) {
        return;
    }

    int n_workers = std::max(world_size - 1, 1);
    int chunk_size = n / n_workers;
    assert(chunk_size <= n);
        
    int remainder = n % n_workers;

    // Let the first rank process the remainder
    if (this_rank == 0){
        // Process eventual smaller chunk first
        for (int i = 0; i < remainder; i++) {
            y[i] += a * x[i];
        }
    
        // Rank 0 collects results from all other ranks
        for (int rank = 1; rank < world_size; rank++) {
            int recv_start_index = remainder + (rank - 1) * chunk_size;
            MPI_Recv(y + recv_start_index, chunk_size, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        for (int i = 0; i < chunk_size; i++) {
            y[i] += a * x[i];
        }
        MPI_Send(y, chunk_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
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

double sum_chunked_parallel(int n, double *x) {
    int world_size, this_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &this_rank);

    if (n <= 0) {
        return 0.0;
    }

    int n_workers = std::max(world_size - 1, 1);
    int chunk_size = n / n_workers;

    int remainder = n % n_workers;
    double* partial_sums = new double[world_size]; // takes into account remainder processed by rank 0

    if (this_rank == 0) {
        if (world_size < 2) {
            // If there is only one rank, just return the sum of the whole array
            partial_sums[0] = KahanBabushkaNeumaierSum(x, n);
        } else {
            // Rank 0 will send chunks to all other ranks
            for (int rank = 1; rank < world_size; rank++) {
                int start_index = remainder + (rank - 1) * chunk_size;
                MPI_Send(x + start_index, chunk_size, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD);
            }
            // Rank 0 processes the eventual remainder
            partial_sums[0] = KahanBabushkaNeumaierSum(x, remainder);
        }
    } else {
        // All other ranks receive their chunk
        delete[] partial_sums; // Clean up memory allocated for partial sums
        MPI_Recv(x, chunk_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Now sum up received data if rank 0
    if (this_rank == 0) {
        for (int i = 1; i < world_size; i++) {
            MPI_Recv(partial_sums + i, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        return KahanBabushkaNeumaierSum(partial_sums, world_size);
    } else {
        // All other ranks compute their partial sums and send to 0
        double local_sum = KahanBabushkaNeumaierSum(x, chunk_size);
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return 0.0; // Return 0, as this rank does not contribute to the final sum
    }
}


int main(int argc, char* argv[]) {
    // Will use MPI with rank 0 as main process, sending data to all other ranks
    // and receiving results back from them.
    MPI_Init(&argc, &argv);

    int world_size, this_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &this_rank);
    if (this_rank == 0) {
        std::cout << "Running with " << world_size << " ranks." << std::endl;
    }

    // Sleep to allow debugger to attach to all ranks if needed
    // Remove this in production code!
    volatile int i = 1;
    while (0 == i)
        sleep(5);
    
    const double TOLERANCE = 1e-10;
    const double a = 3.;
    const size_t ARRAY_SIZES[] = {10, 1000, 10000, 1000000, 100000000};

    // Test memory allocation on the stack and heap
    // for each array size and implementation of daxpy
    for (const size_t n: ARRAY_SIZES) {

        // Initialize on rank 0 and send a chunk to all
        if (this_rank == 0) {
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "Testing with n = " << n << std::endl;
        }

        int chunk_size = n / std::max(world_size-1, 1);

        // Allocate memory on the heap for each rank, spare some messages
        double *x, *y;
        if (this_rank == 0) {
            x = new double[n];
            y = new double[n];
        } else {
            x = new double[chunk_size];
            y = new double[chunk_size];
        }

        // Check if memory allocation was successful
        if (x == nullptr || y == nullptr) {
            std::cerr << "Memory allocation failed on rank " << this_rank<< std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        // Initialize x and y
        for (size_t j = 0; j < (this_rank == 0 ? n : chunk_size); j++) {
            x[j] = 0.1;
            y[j] = 7.1;
        }

        // Perform daxpy with MPI
        auto start = std::chrono::high_resolution_clock::now();
        daxpy_chunked_parallel(n, a, x, y);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (this_rank == 0) {
            std::cout << "daxpy time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result (only on rank 0, the others have partial results!)
            for (size_t j = 0; j < n; j++) {
                if (fabs(y[j] - (7.1 + a * 0.1)) > TOLERANCE) {
                    std::cerr << "Error in daxpy result at index " << j << ": " << y[j] << " != " << (7.1 + a * 0.1) << std::endl;
                    MPI_Abort(MPI_COMM_WORLD, 1);
                    return 1;
                }
            }

            for (size_t j = 0; j < n; j++) {
                y[j] = 7.1;
            }
            start = std::chrono::high_resolution_clock::now();
            daxpy_chunked(n, a, x, y, chunk_size);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "daxpy (no MPI) time: " << elapsed.count() << " seconds" << std::endl;
        }

        // sum with MPI
        start = std::chrono::high_resolution_clock::now();
        auto sum = sum_chunked_parallel(n, y);
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        if (this_rank == 0) {
            std::cout << "sum time: " << elapsed.count() << " seconds" << std::endl;
            // Verify result
            if (fabs(sum - n * (7.1 + a * 0.1)) > n*TOLERANCE) {
                std::cerr << "Error in sum result: " << sum << " != " << n * (7.1 + a * 0.1) << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
                return 1;
            }

            start = std::chrono::high_resolution_clock::now();
            sum = sum_chunked(n, y, chunk_size);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "sum (no MPI) time: " << elapsed.count() << " seconds" << std::endl;
        }

        delete[] x;
        delete[] y;
        
        if (this_rank == 0) {
            std::cout << "----------------------------------------" << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}
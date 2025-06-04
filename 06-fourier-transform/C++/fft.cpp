#include <iomanip>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <fftw3.h>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>

// Function to calculate absolute mean square error
double calculateMeanSquareErrorAbs(const Eigen::MatrixXd& original, const Eigen::MatrixXd& reconstructed) {
    Eigen::MatrixXd diff = original - reconstructed;
    return std::sqrt(diff.array().square().mean());
}

// Function to calculate absolute median square error
double calculateMedianSquareErrorAbs(const Eigen::MatrixXd& original, const Eigen::MatrixXd& reconstructed) {
    Eigen::MatrixXd diff = original - reconstructed;
    std::vector<double> squaredErrors(diff.size());
    for (int i = 0; i < diff.size(); ++i) {
        squaredErrors[i] = std::pow(diff(i), 2);
    }
    std::nth_element(squaredErrors.begin(), squaredErrors.begin() + squaredErrors.size() / 2, squaredErrors.end());
    return std::sqrt(squaredErrors[squaredErrors.size() / 2]);
}

// Function to calculate relative mean square error
double calculateMeanSquareErrorRel(const Eigen::MatrixXd& original, const Eigen::MatrixXd& reconstructed) {
    Eigen::MatrixXd diff = original - reconstructed;
    auto diff_rel = diff.array() / original.array();
    return std::sqrt(diff_rel.array().square().mean());
}

// Function to calculate relative median square error
double calculateMedianSquareErrorRel(const Eigen::MatrixXd& original, const Eigen::MatrixXd& reconstructed) {
    Eigen::MatrixXd diff = original - reconstructed;
    auto diff_rel = diff.array() / original.array();
    std::vector<double> squaredErrors(diff_rel.size());
    for (int i = 0; i < diff_rel.size(); ++i) {
        squaredErrors[i] = std::pow(diff_rel(i), 2);
    }
    std::nth_element(squaredErrors.begin(), squaredErrors.begin() + squaredErrors.size() / 2, squaredErrors.end());
    return std::sqrt(squaredErrors[squaredErrors.size() / 2]);
}

int main() {
    const int N = 6; // Matrix size
    Eigen::MatrixXd A(N, N);

    // Step 1: Fill matrix A with Gaussian random variables (mean=1, stddev=1)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(1.0, 1.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A(i, j) = d(gen);
        }
    }

    // Step 2: Perform FFT c2c (complex-to-complex)
    fftw_complex* in_c2c = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
    fftw_complex* C = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
    fftw_plan plan_c2c = fftw_plan_dft_2d(N, N, in_c2c, C, FFTW_FORWARD, FFTW_ESTIMATE);

    // Fill input for FFT
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            in_c2c[i * N + j][0] = A(i, j); // Real part
            in_c2c[i * N + j][1] = 0.0;    // Imaginary part
        }
    }

    fftw_execute(plan_c2c); // Execute FFT

    // Reconstruct matrix A by inverse FFT c2c
    fftw_plan plan_c2c_inverse = fftw_plan_dft_2d(N, N, C, in_c2c, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan_c2c_inverse);

    Eigen::MatrixXd A_reconstructedC(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A_reconstructedC(i, j) = in_c2c[i * N + j][0] / (N * N); // Normalize by size
        }
    }

    // Calculate errors for c2c
    double AbsMeanSquareError_c2c = calculateMeanSquareErrorAbs(A, A_reconstructedC);
    double AbsMedianSquareError_c2c = calculateMedianSquareErrorAbs(A, A_reconstructedC);
    double RelMeanSquareError_c2c = calculateMeanSquareErrorRel(A, A_reconstructedC);
    double RelMedianSquareError_c2c = calculateMedianSquareErrorRel(A, A_reconstructedC);

    // Step 3: Perform FFT r2c (real-to-complex)
    Eigen::MatrixXd A_reconstructed_r2c(N, N);
    fftw_complex* R = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * (N / 2 + 1));
    fftw_plan plan_r2c = fftw_plan_dft_r2c_2d(N, N, A.data(), R, FFTW_ESTIMATE);

    fftw_execute(plan_r2c); // Execute FFT

    // Reconstruct matrix A by inverse FFT r2c
    fftw_plan plan_r2c_inverse = fftw_plan_dft_c2r_2d(N, N, R, A_reconstructed_r2c.data(), FFTW_ESTIMATE);
    fftw_execute(plan_r2c_inverse);

    // Normalize by size
    A_reconstructed_r2c /= (N * N);

    // Calculate errors for r2c
    double AbsMeanSquareError_r2c = calculateMeanSquareErrorAbs(A, A_reconstructed_r2c);
    double AbsMedianSquareError_r2c = calculateMedianSquareErrorAbs(A, A_reconstructed_r2c);
    double RelMeanSquareError_r2c = calculateMeanSquareErrorRel(A, A_reconstructed_r2c);
    double RelMedianSquareError_r2c = calculateMedianSquareErrorRel(A, A_reconstructed_r2c);

    // Step 5: Check if errors are within machine precision
    std::cout << "Mean Square Error (c2c) (abs - rel):\t" << AbsMeanSquareError_c2c << " - " << RelMeanSquareError_c2c << std::endl;
    std::cout << "Median Square Error (c2c):\t\t" << AbsMedianSquareError_c2c << " - " << RelMedianSquareError_c2c << std::endl;
    std::cout << "Mean Square Error (r2c):\t\t" << AbsMeanSquareError_r2c << " - " << RelMeanSquareError_r2c << std::endl;
    std::cout << "Median Square Error (r2c):\t\t" << AbsMedianSquareError_r2c << " - " << RelMedianSquareError_r2c << std::endl;

    // Step 6: Value of C[0,0] and R[0,0]
    std::cout << "C[0,0] (normalized): " << C[0][0] / (N*N) << " + " << C[0][1] / (N*N) << "i" << std::endl;
    std::cout << "R[0,0] (normalized): " << R[0][0] / (N*N) << " + " << R[0][1] / (N*N) << "i" << std::endl;

    if (N == 6) {
        std::cout << "Compare C and R for N=6:" << std::endl;
        std::cout << "C:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << std::setprecision(4) << C[i * N + j][0] / (N*N) << "+" << std::setprecision(4) << C[i * N + j][1] / (N*N) << "i ";
            }
            std::cout << std::endl;
        }
        std::cout << "R:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N / 2 + 1; ++j) {
                std::cout << std::setprecision(4) << R[i * (N / 2 + 1) + j][0] / (N*N) << "+" << std::setprecision(4) << R[i * (N / 2 + 1) + j][1] / (N*N) << "i ";
            }
            std::cout << std::endl;
        }
        
        Eigen::MatrixXcd C_eigen(N, N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                C_eigen(i, j) = std::complex<double>(C[i * N + j][0] / (N * N), C[i * N + j][1] / (N * N));
            }
        }
        Eigen::MatrixXcd R_eigen(N, N / 2 + 1);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N / 2 + 1; ++j) {
                R_eigen(i, j) = std::complex<double>(R[i * (N / 2 + 1) + j][0] / (N*N), R[i * (N / 2 + 1) + j][1] / (N*N));
            }
        }
        std::cout << "C determinant: " << C_eigen.determinant() << std::endl;
        // std::cout << "R determinant: " << R_eigen.determinant() << std::endl;
    }

    // Cleanup
    fftw_destroy_plan(plan_c2c);
    fftw_destroy_plan(plan_c2c_inverse);
    fftw_destroy_plan(plan_r2c);
    fftw_destroy_plan(plan_r2c_inverse);
    fftw_free(in_c2c);
    fftw_free(C);
    fftw_free(R);

    return 0;
}
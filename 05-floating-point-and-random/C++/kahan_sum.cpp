#include <iomanip>
#include <iostream>
#include <eigen3/Eigen/Dense>

using namespace std;

double naive_sum(const double *vec, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += vec[i];
    }
    return sum;
}

double Kahan_sum(const double *vec, int n) {
    /*
    Kahan summation algorithm
    This algorithm compensates for lost low-order bits by keeping a running compensation
    for lost low-order bits. It still fails when the term to sum is much larger than the 
    accumulated sum, or better when the sum is ill-conditioned, with \sum |x_i| >> |\sum x_i|.
    */

    double sum = 0.0, c = 0.0;

    for (int i = 0; i < n; i++) {
        double y = vec[i] - c; // So far, so good: c is zero.
        double t = sum + y; // Alas, sum is big, y small, so low-order digits of y are lost.
        c = (t - sum) - y; // (t - sum) recovers the high-order part of y; subtracting y recovers the low-order part.
        sum = t; // Algebraically, c should always be zero.
    }
    return sum;
}

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

int main() {

    // Initialize arrays for the three summation algorithms
    // for loop
    const int n = 4;
    double vec[] = {1.0, 1.0e16, -1.0e16, -0.5};
    
    // eigen3 sum
    Eigen::Vector4d eigen_vec(1.0, 1.0e16, -1.0e16, -0.5);

    // Calculate the sums
    double naive_result = naive_sum(vec, n);
    double kahan_result = Kahan_sum(vec, n);
    double kahan_babushka_result = KahanBabushkaNeumaierSum(vec, n);
    double eigen_result = eigen_vec.sum();

    // Print the results
    cout << "Naive sum: " << naive_result << endl;
    cout << "Kahan sum: " << kahan_result << endl;
    cout << "Kahan-Babushka-Neumaier sum: " << kahan_babushka_result << endl;
    cout << "eigen3 sum: " << eigen_result << endl;
    
    return 0;
}
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <boost/program_options.hpp>
#include <gsl/gsl_integration.h>

namespace po = boost::program_options;
using namespace std;

// Function definition: f(x) = exp(x) * cos(x)
double f(double x, void* params) {
    return exp(x) * cos(x);
}

double fo(double x, void* params) {
    return exp(x);
}

// Function to calculate the integral of f(x) using the trapezoidal rule
double trapezoidal_rule(double (*f)(double, void*), double a, double b, int N, void* params) {
    double h = (b - a) / N;
    double sum = 0.5 * (f(a, params) + f(b, params));
    for (int i = 1; i < N; ++i) {
        sum += f(a + i * h, params);
    }
    return sum * h;
}

int main(int argc, char *argv[]) {
    // Input: Number of sampling points, and domain limits
    int N;
    double x_inf, x_sup;
    double true_value = (exp(M_PI_2) - 1.0) / 2.0;
    
    // Using Boost Program Options to handle command line arguments
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("N,n", po::value<int>(&N)->default_value(100), "number of sampling points")
        ("x_inf,i", po::value<double>(&x_inf)->default_value(0.0), "lower limit of integration")
        ("x_sup,s", po::value<double>(&x_sup)->default_value(M_PI_2), "upper limit of integration");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    if (N <= 0) {
        cerr << "Error: N must be a positive integer.\n";
        return 1;
    }
    if (x_inf >= x_sup) {
        cerr << "Error: x_inf must be less than x_sup.\n";
        return 1;
    }


    // Generate the sampling points and save to a file
    ofstream output_file("output.txt");
    if (!output_file.is_open()) {
        cerr << "Error: Could not open file for writing.\n";
        return 1;
    }

    double step = (x_sup - x_inf) / (N - 1);
    double x, fx;
    output_file << fixed << setprecision(8);
    for (int i = 0; i < N; ++i) {
        x = x_inf + i * step;
        fx = f(x, nullptr);
        output_file << x << "\t" << fx << "\n";
    }
    output_file.close();
    
    cout << "Sampling points saved to 'output.txt'.\n";
    
    cout << "The integral I = ∫ f(x) dx from 0 to pi/2 is: " << endl;
    cout << "\t True value: " << setprecision(16) << true_value << endl;
    // Calculate the integral of f(x) from 0 to pi/2 using GSL QAG
    gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(N);
    gsl_function F;
    F.function = &f;
    F.params = nullptr;

    double result, error, epsrel;
    gsl_integration_qag(&F, 0, M_PI_2, 1e-8, 1e-8, N, GSL_INTEG_GAUSS61, workspace, &result, &error);
    gsl_integration_workspace_free(workspace);

    // Print the result with 16 decimal digits
    epsrel = result / true_value - 1.0;
    cout << "\t QAG: " << setprecision(16) << result << " (" << epsrel << ")" << endl;

    // Calculate the integral of f(x) from 0 to pi/2 using GSL QAWO
    workspace = gsl_integration_workspace_alloc(N);
    gsl_integration_qawo_table * wf = gsl_integration_qawo_table_alloc(1.0, M_PI_2, GSL_INTEG_COSINE, N);
    F.function = &fo;
    F.params = nullptr;

    gsl_integration_qawo(&F, 0, 1e-8, 1e-8, N, workspace, wf, &result, &error);
    gsl_integration_qawo_table_free(wf);
    gsl_integration_workspace_free(workspace);

    epsrel = result / true_value - 1.0;
    cout << "\t QAWO: " << setprecision(16) << result << " (" << epsrel << ")" << endl;

    // Calculate the integral of f(x) from 0 to pi/2 using trapezoidal rule
    result = trapezoidal_rule(f, 0, M_PI_2, N, nullptr);

    epsrel = result / true_value - 1.0;
    cout << "\t Trapezoidal Rule: " << setprecision(16) << result << " (" << epsrel << ")" << endl;

    // Save the results to a file
    ofstream result_file("result.txt", ios::app);
    if (!result_file.is_open()) {
        cerr << "Error: Could not open file for writing.\n";
        return 1;
    }

    result_file << fixed << setprecision(16);
    result_file << N << "\t" << result << "\t" << epsrel << "\n";
    result_file.close();
    cout << "Results saved to 'result.txt'.\n";
    return 0;
}
#include <iostream>
#include <fstream>
#include <array>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

using namespace std;

const string OUTPUT_DIR = "output";
const string OUTPUT_FILE = OUTPUT_DIR + "/output_cpp.txt";

int main(int argc, char *argv[]) {
    // Create the output directory
    system(("mkdir -p " + OUTPUT_DIR).c_str());

    // Define the input values
    double a = 3;
    const size_t n = 20;
    array<double, n> x;
    array<double, n> y;
    array<double, n> z;

    // Fill the input arrays
    x.fill(1.0);
    y.fill(4.0);

    // Calculate z = a*x + y
    for (int i = 0; i < 20; i++) {
        z[i] = a * x[i] + y[i];
    }

    // Write the result to a text file
    ofstream ofs(OUTPUT_FILE);
    if (!ofs) {
        std::cerr << "Error: Unable to open the output file\n";
        return 1;
    }
    for (double val : z) {
        ofs << val << std::endl;
    }
    ofs.close();

    return 0;
}
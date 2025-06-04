#include <iostream>
#include <boost/program_options.hpp>

#include "fileio.hpp"

namespace po = boost::program_options;

void generate_vector(int N, double * &v, double init_value) {
    // allocate memory for vector if not already allocated
    if (v == nullptr) {
        v = new double[N];
        if (v == nullptr) {
            std::cerr << "Memory allocation failed\n";
            exit(EXIT_FAILURE);
        }
    }

    // else we assume the memory is already allocated 
    // and initialize vector
    for (int i = 0; i < N; i++) *(v + i) = init_value;
}

int main(int argv, char *argc[]) {

    std::cout << "Hi there! " << argc[0] << " is running..." << std::endl;

    // parse command line
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("N,n", po::value<int>()->default_value(10), "size of vectors")
        ("fname_prefix,f", po::value<std::string>()->default_value("vector"), "prefix for output files");
    po::variables_map vm;

    po::store(po::parse_command_line(argv, argc, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (!vm.count("N") || !vm.count("fname_prefix")) {
        std::cout << "Missing required arguments.\n";
        std::cout << desc << std::endl;
        return 1;
    }

    // read command line arguments
    std::cout << "Command line arguments:\n";
    int N = vm["N"].as<int>();
    std::string fname_prefix = vm["fname_prefix"].as<std::string>();
    std::cout << "Generating vectors with N=" << N << " elements on files " << fname_prefix << std::endl;

    double *vec = nullptr;
    std::cout << "Generating vector X ..." << std::endl;
    generate_vector(N, vec, 0.1);

    std::string filename = fname_prefix + "_N" + std::to_string(N) + "_x.dat";
    dump_vector_binary(N, filename, vec);

    std::cout << "Generating vector Y ...\n";
    generate_vector(N, vec, 7.1);
    filename = fname_prefix + "_N" + std::to_string(N) + "_y.dat";
    dump_vector_binary(N, filename, vec);
    // clean up
    delete[] vec;
    return 0;

}
#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <iostream>
#include <string>
#include <hdf5.h>

void read_vector_hdf5(int N, const std::string fname, const std::string dataset_name, double * &vector) {
    hid_t file_id, dataset_id;
    herr_t status;

    // IO
    file_id = H5Fopen(fname.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    dataset_id = H5Dopen(file_id, dataset_name.c_str(), H5P_DEFAULT);

    // allocate memory
    if (vector == nullptr) {
        vector = new double[N];
        if (vector == nullptr) {
            std::cout << "Memory allocation failed" << std::endl;
            H5Dclose(dataset_id);
            H5Fclose(file_id);
            exit(EXIT_FAILURE);
        }
    }

    // read from file
    status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector);

    // close
    H5Dclose(dataset_id);
    H5Fclose(file_id);

    if (status < 0) {
        std::cout << "Error reading dataset " << dataset_name << " from file " << fname << std::endl;
        delete[] vector;
        vector = nullptr;
        exit(EXIT_FAILURE);
    }
}

void dump_vector_hdf5(int N, const std::string fname, const std::string dataset_name, const double *vect) {
    hid_t file_id, dataset_id, dataspace_id;
    hsize_t dims[1] = {(hsize_t)N};

    std::cout << "Writing file <" << fname << "> ..." << std::endl;

    // create a new file & dataset
    file_id = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    dataspace_id = H5Screate_simple(1, dims, NULL);
    dataset_id = H5Dcreate(file_id, dataset_name.c_str(), H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // write
    H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vect);

    // closing operations
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);

    std::cout << "done." << std::endl;
}

#endif // FILEIO_HPP
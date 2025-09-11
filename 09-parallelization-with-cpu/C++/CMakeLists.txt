find_package(OpenMP)

add_executable(09daxpyCpp_OMP parallel_daxpy_omp.cpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(09daxpyCpp_OMP PUBLIC OpenMP::OpenMP_CXX)
endif()

set(MPI_HOME "/usr/lib64/openmpi")
find_package(MPI)

if(MPI_FOUND)
    add_executable(09daxpyCpp_MPI parallel_daxpy_mpi.cpp)
    target_link_libraries(09daxpyCpp_MPI PUBLIC MPI::MPI_CXX)
endif()
    
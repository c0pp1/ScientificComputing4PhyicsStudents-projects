cmake_minimum_required(VERSION 3.14)
project(SC4PS)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip

)
FetchContent_MakeAvailable(googletest)

enable_testing()

# compile the source code in each directory
add_subdirectory(01-hello-world/C++)
add_subdirectory(01-hello-world/C)
add_subdirectory(02-linear-algebra/C++)
add_subdirectory(02-linear-algebra/C)
add_subdirectory(03-code-io/C)
add_subdirectory(03-code-io/C++)
add_subdirectory(04-discrete-math/C++)
add_subdirectory(05-floating-point-and-random/C++)
add_subdirectory(06-fourier-transform/C++)
add_subdirectory(07-unit-testing/C++)
add_subdirectory(08-split-the-work/C++)
add_subdirectory(09-parallelization-with-cpu/C++)
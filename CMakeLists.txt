cmake_minimum_required(VERSION 3.0)
project(SC4PS)

set(CMAKE_CXX_STANDARD 11)

# include boost program options
set(BOOSTROOT /usr/lib64)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# compile the source code in each directory
add_subdirectory(01-hello-world)

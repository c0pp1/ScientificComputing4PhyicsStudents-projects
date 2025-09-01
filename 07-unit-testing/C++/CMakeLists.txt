cmake_minimum_required(VERSION 3.14)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(07unittestCpp daxpy_test.cpp)
target_link_libraries(
  07unittestCpp
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(07unittestCpp)
find_package(OpenMP)

add_executable(09daxpyCpp parallel_daxpy.cpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(09daxpyCpp PUBLIC OpenMP::OpenMP_CXX)
endif()
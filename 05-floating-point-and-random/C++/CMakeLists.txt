
add_executable(05kahan_sumC++ kahan_sum.cpp)

add_executable(05genC++ generator.cpp)
target_compile_options(05genC++ PRIVATE -std=c++14 -fpermissive)

add_executable(05daxpyC++ daxpy_from_config.cpp)
target_compile_options(05daxpyC++ PRIVATE -fpermissive)

add_custom_target(run-05daxpy-randomC++ WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/05genC++ COMMAND ${CMAKE_CURRENT_BINARY_DIR}/05daxpyC++)
add_dependencies(run-05daxpy-randomC++ 05genC++ 05daxpyC++)
# include boost program options
set(BOOSTROOT /usr/lib64)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)

add_executable(03genC++ generator.cpp)
target_compile_options(03genC++ PRIVATE -std=c++14)
target_include_directories(03genC++ PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(03genC++ ${Boost_LIBRARIES})
add_executable(03dax-ioC++ daxpy_from_config.cpp)
target_compile_options(03dax-ioC++ PRIVATE -fpermissive)

add_custom_target(run-03code-ioC++ WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03genC++ -n 10 -f vector)
add_dependencies(run-03code-ioC++ 03genC++ 03dax-ioC++)
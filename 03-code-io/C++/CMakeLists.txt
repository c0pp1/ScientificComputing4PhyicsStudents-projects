# include boost program options
set(BOOSTROOT /usr/lib64)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)

add_executable(03genC++ generator.cpp)
target_compile_options(03genC++ PRIVATE -std=c++14)
target_include_directories(03genC++ PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(03genC++ ${Boost_LIBRARIES})
add_executable(03dax-ioC++ daxpy_from_config.cpp)
target_compile_options(03dax-ioC++ PRIVATE -fpermissive)

add_custom_target(run-03code-ioC++ WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03genC++ -n 10 -f vector COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03dax-ioC++)
add_dependencies(run-03code-ioC++ 03genC++ 03dax-ioC++)

# gsl
find_package(GSL)
if(GSL_FOUND)
    include_directories(${GSL_INCLUDE_DIRS} )
    add_executable(03dax-ioC++_gls gsl/gsl.axpy.cpp)
    target_link_libraries(03dax-ioC++_gls ${GSL_LIBRARIES})
else()
    message(STATUS "GSL not found, skipping dax-io.glsC++")
endif()
# hdf5
find_package(HDF5)
if(HDF5_FOUND)
    include_directories(${HDF5_INCLUDE_DIRS})
    add_executable(03dax-ioC++_hdf5 hdf5/daxpy_from_config.cpp)
    add_executable(03genC++_hdf5 hdf5/generator.cpp)

    target_link_libraries(03dax-ioC++_hdf5 ${HDF5_LIBRARIES})
    target_link_libraries(03genC++_hdf5 ${HDF5_LIBRARIES} ${Boost_LIBRARIES})
    target_include_directories(03genC++_hdf5 PUBLIC ${Boost_INCLUDE_DIRS})

    target_compile_options(03dax-ioC++_hdf5 PRIVATE -fpermissive)
    
    add_custom_target(run-03code-ioC++_hdf5 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03genC++_hdf5 10 vector COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03dax-ioC++_hdf5 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/hdf5)
    add_dependencies(run-03code-ioC++_hdf5 03genC++_hdf5 03dax-ioC++_hdf5)
    #add_test(NAME 03code-ioC++_hdf5 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/03genC++_hdf5 10 vector && ${CMAKE_CURRENT_BINARY_DIR}/dax-ioC++_hdf5 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/hdf5)
else()
    message(STATUS "HDF5 not found, skipping dax-io.hdf5C")
endif()
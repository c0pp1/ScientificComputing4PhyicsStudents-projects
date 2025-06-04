# include boost program options
set(BOOSTROOT /usr/lib64)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
# gsl
find_package(GSL)

if(GSL_FOUND AND Boost_FOUND)
    include_directories(${GSL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
    add_executable(04integrateC++ integral_solver.cpp)
    target_link_libraries(04integrateC++ ${Boost_LIBRARIES} ${GSL_LIBRARIES})

    add_custom_target(run-04integrateC++ COMMAND ${CMAKE_CURRENT_BINARY_DIR}/04integrateC++ -n 1000 -i 0 -s 2 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
else()
    message(STATUS "GSL or BOOST not found, skipping 04integrateC++")
endif()


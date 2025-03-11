# Path: 01-hello-world/CMakeLists.txt
add_executable(hello-world main.cpp)
target_link_libraries(hello-world ${Boost_LIBRARIES})

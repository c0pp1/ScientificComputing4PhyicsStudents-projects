add_executable(06-fftC++ fft.cpp)
target_include_directories(06-fftC++ PRIVATE /usr/local)
target_link_libraries(06-fftC++ PRIVATE fftw3)

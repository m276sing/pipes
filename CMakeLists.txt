cmake_minimum_required(VERSION 2.8)
project(pipes)

## ECE650 Specific configuration start here
## Project specific configurations go here



# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)



# create the main executable
## add additional .cpp files if needed
add_executable(pipes pipes_example.cpp)
add_executable(a a.cpp )

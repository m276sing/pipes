cmake_minimum_required(VERSION 2.8)
project(project)

## ECE650 Specific configuration start here
## Project specific configurations go here

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


include_directories(${CMAKE_SOURCE_DIR}/minisat)


# create the main executable
## add additional .cpp files if needed
add_executable(project project.cpp)
add_executable(a a.cpp)
add_executable(testt test.cpp)
add_executable(b b.cpp)
add_executable(ran randomdata.cpp)

target_link_libraries(ran minisat-lib-static)
target_link_libraries(project minisat-lib-static)

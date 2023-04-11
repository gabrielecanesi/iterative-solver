cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


project(iterative-client)

find_package (Eigen3 REQUIRED NO_MODULE)

add_executable(iterative-client mainCLI.cpp)
add_subdirectory(../lib ../lib)
include_directories(../lib)

target_link_libraries (iterative-client PRIVATE Eigen3::Eigen iterative_solver)
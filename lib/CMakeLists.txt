cmake_minimum_required(VERSION 3.26)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


project(iterative_solver)

find_package (Eigen3 REQUIRED NO_MODULE)

add_library(iterative_solver STATIC timer.cpp)
target_link_libraries (iterative_solver Eigen3::Eigen)
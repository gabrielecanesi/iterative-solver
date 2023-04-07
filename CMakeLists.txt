cmake_minimum_required(VERSION 3.20)
project(iterative_solver)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(iterative_solver
        GradientUpdateStrategy.h
        IterativeSolver.h
        main.cpp
        MatrixReader.h
        Solver.h
        UpdateStrategy.h)

file(COPY ${CMAKE_SOURCE_DIR}/Matrices DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


project(iterative_solver)

include(FetchContent)
FetchContent_Declare(eigen                   
 GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
 GIT_TAG        3.4.0
)

FetchContent_GetProperties(Eigen)

if(NOT eigen_POPULATED)
  FetchContent_Populate(Eigen)
endif()


include_directories(${eigen_SOURCE_DIR})
add_library(iterative_solver STATIC util/timer.cpp exceptions/WrongParameterValueException.cpp util/MatrixReader.cpp)
target_link_libraries(iterative_solver)
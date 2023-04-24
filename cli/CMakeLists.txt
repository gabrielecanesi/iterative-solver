cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


project(iterative-client)

include(FetchContent)
FetchContent_Declare(eigen                   
 GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
 GIT_TAG        3.4.0
)

FetchContent_GetProperties(Eigen)
if(NOT eigen_POPULATED)
  FetchContent_Populate(Eigen)
endif()



add_executable(iterative-client mainCLI.cpp command/Solve.cpp command/Test.cpp exceptions/FileNotFoundException.h)
add_subdirectory(../lib ../build-lib)
include_directories(../lib ${eigen_SOURCE_DIR})


target_link_libraries (iterative-client PRIVATE iterative_solver)
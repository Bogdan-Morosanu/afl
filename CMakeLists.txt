cmake_minimum_required(VERSION 3.10)

project(afl)

add_subdirectory(include)
add_subdirectory(src)

find_package(BLAS)

message(${BLAS_FOUND})
message("^^^")

add_executable(afl main.cpp)
target_include_directories(afl PUBLIC "${PROJECT_SOURCE_DIR}/include" ${BLAS_INCLUDE_DIRS})
target_link_libraries(afl PUBLIC BLAS::BLAS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

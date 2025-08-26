cmake_minimum_required(VERSION 3.11)

project(sandbox)

set(CMAKE_CXX_STANDARD 23)

link_directories(${CMAKE_BINARY_DIR}/lambda)

link_libraries(-llambda)

include_directories(${CMAKE_SOURCE_DIR}/lambda/src)

add_executable(sandbox src/main.cpp)
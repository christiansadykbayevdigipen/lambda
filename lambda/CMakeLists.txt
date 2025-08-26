cmake_minimum_required(VERSION 3.11)

project(lambda)

set(CMAKE_CXX_STANDARD 23)

link_directories(${CMAKE_BINARY_DIR}/SDL/)

link_libraries(-lSDL3)

include_directories(${CMAKE_SOURCE_DIR}/SDL/include)

add_library(lambda SHARED src/example.cpp)
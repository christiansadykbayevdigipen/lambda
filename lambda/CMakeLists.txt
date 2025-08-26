cmake_minimum_required(VERSION 3.11)

project(lambda)

set(CMAKE_CXX_STANDARD 23)

add_library(lambda SHARED src/example.cpp)
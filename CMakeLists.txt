cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(euler_skeleton VERSION 1.0)
add_executable(euler main.cpp)

install(TARGETS euler DESTINATION bin)

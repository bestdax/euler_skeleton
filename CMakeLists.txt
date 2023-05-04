cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project_template VERSION 1.0)
add_executable(prt main.cpp prt.cpp)

install(TARGETS prt DESTINATION bin)

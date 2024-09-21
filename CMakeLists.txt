cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

project(project_template VERSION 1.0)
find_package(nlohmann_json REQUIRED)

add_executable(prt main.cpp prt.cpp)
target_link_libraries(prt PRIVATE nlohmann_json::nlohmann_json)
install(TARGETS prt DESTINATION bin)

#include "prt.h"

void ProjectTemplate::load_templates()
{
	main_cmake_content = R""""( cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project_euler VERSION 1.0)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(googletest)
)"""";
	subdir_cmake_content = R""""(file(
  GLOB SUBDIRS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *)
foreach(SUBDIR ${SUBDIRS})
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
    add_subdirectory(${SUBDIR})
  endif()
endforeach()
)"""";
	problem_cmake_content = R""""(get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  set(targetname ${CURRENT_DIR_NAME})
  add_executable(${targetname} main.cpp problem.cpp
)"""";
  problem_test_cmake_content = R""""(get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  set(targetname test_${CURRENT_DIR_NAME})
  add_executable(${targetname} test.cpp ${PROJECT_SOURCE_DIR}/src/${CURRENT_DIR_NAME}/problem.cpp)
  target_include_directories(${targetname} PUBLIC ${PROJECT_SOURCE_DIR}/googletest/googletest/include)
  target_include_directories(${targetname} PUBLIC ${PROJECT_SOURCE_DIR}/src/${CURRENT_DIR_NAME}/)
  target_link_libraries(${targetname} gtest_main)
)"""";
  main_cpp_content = R""""(#include "problem.h"

  int main(int argc, char *argv[])
  {
    Solution solution;
    solution.answer();
    return 0;
  }
)"""";

	time_t _tm = time(NULL);
	struct tm * curtime = localtime(&_tm);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", curtime);
	std::string time_str(buffer);
	problem_cpp_content = R""""(/*
Problem description:

page link:
https://projecteuler.net/problem=

dax )"""";
	problem_cpp_content += time_str + '\n';
	problem_cpp_content += R""""(*/
#include "problem.h"

void Solution::answer()
{
	std::cout << "The answer is: " << "" << std::endl;
}
)"""";
  problem_test_cpp_content = R""""(#include "gtest/gtest.h"
#include "problem.h"

  Solution solution;

  int main(int argc, char *argv[])
  {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
  }
)"""";
  problem_header_content = R""""(#pragma once
#include <iostream>

  class Solution
  {
  public:
    void answer();

  };
)"""";
}

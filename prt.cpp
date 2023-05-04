#include "prt.h"

ProjectTemplate::ProjectTemplate()
{
	project_dir = std::filesystem::current_path();
	main_cmake_content = R""""(cmake_minimum_required(VERSION 3.25)
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

void ProjectTemplate::mkdir(std::filesystem::path path)
{
	std::string mkdir = "mkdir -p ";
	mkdir += path;
	std::system(mkdir.c_str());
}

void ProjectTemplate::create_file(std::filesystem::path path, std::string content)
{
	std::ofstream file(path);
	file << content << std::endl;
}

void ProjectTemplate::create_euler_project(std::string prj_name)
{
	if(project_dir.filename() != "projecteuler") return;

	// create dirs
	std::filesystem::path src_dir{project_dir}, test_dir{project_dir};
	
	src_dir /= "src";
	test_dir /= "tests";

	if(!std::filesystem::is_directory(src_dir))
		mkdir(src_dir);

	if(!std::filesystem::is_directory(test_dir))
		mkdir(test_dir);

	std::filesystem::path problem_dir{src_dir}, problem_test_dir{test_dir};
	problem_dir /= prj_name;
	problem_test_dir /= prj_name;

	if(!std::filesystem::is_directory(problem_dir))
		mkdir(problem_dir);

	if(!std::filesystem::is_directory(problem_test_dir))
		mkdir(problem_test_dir);

	// create main project CMakeLists.txt
	std::filesystem::path main_cmake_path{project_dir};
	main_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(main_cmake_path))
		create_file(main_cmake_path, main_cpp_content);

	// create subdir CMakeLists.txt
	std::filesystem::path src_cmake_path{src_dir};
	src_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(src_cmake_path))
		create_file(src_cmake_path, subdir_cmake_content);

	std::filesystem::path test_cmake_path{test_dir};
	test_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(test_cmake_path))
		create_file(test_cmake_path, subdir_cmake_content);


	// create problem CMakeLists.txt
	std::filesystem::path problem_cmake_path{problem_dir};
	problem_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(problem_cmake_path))
		create_file(problem_cmake_path, problem_cmake_content);

	// create problem test CMakeLists.txt
	std::filesystem::path problem_test_cmake_path{problem_test_dir};
	problem_test_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(problem_test_cmake_path))
		create_file(problem_test_cmake_path, problem_test_cmake_content);

	// create problem header file
	std::filesystem::path problem_header_path{problem_dir};
	problem_header_path /= "problem.h";

	if(!std::filesystem::is_regular_file(problem_header_path))
		create_file(problem_header_path, problem_header_content);

	// create problem cpp file
	std::filesystem::path problem_cpp_path{problem_dir};
	problem_cpp_path /= "problem.cpp";

	if(!std::filesystem::is_regular_file(problem_cpp_path))
		create_file(problem_cpp_path, problem_cpp_content);

	// create problem test cpp file
	std::filesystem::path problem_test_cpp_path{problem_test_dir};
	problem_test_cpp_path /= "test.cpp";

	if(!std::filesystem::is_regular_file(problem_test_cpp_path))
		create_file(problem_test_cpp_path, problem_test_cpp_content);
}

void ProjectTemplate::create_algorithm_project()
{
}

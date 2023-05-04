#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>



std::string path()
{

}

void mk_problem_and_test_dir(std::string dirname)
{
	std::string src{"src/"};
	src += dirname;
	std::string tests{"tests/"};
	tests += dirname;
	mkdir(src);
	mkdir(tests);
}

void create_problem_cpp_file(std::string filename)
{
	std::string content = R""""(#include "problem.h"

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
	std::string src_problem_cpp_content = R""""(/*
Problem description:

page link:
https://projecteuler.net/problem=

dax )"""";
	src_problem_cpp_content += time_str + '\n';
	src_problem_cpp_content += R""""(*/
#include "problem.h"

void Solution::answer()
{
	std::cout << "The answer is: " << "" << std::endl;
}
)"""";
	newfile(filename, content);
}

int main(int argc, char *argv[])
{
	if(argc < 3) return 1;


	if(strcmp(argv[1], "euler") == 0)
	{
	// check if the working directory is projecteuler
	auto current_path = std::filesystem::current_path();
	if(current_path.filename() != "projecteuler") return 1;
	// create directories
	mk_problem_and_test_dir(argv[2]);
	// create files
	create_problem_cpp_file(argv[2]);
	std::string src_problem_header_content = R""""(#pragma once
#include <iostream>

class Solution
{
public:
	void answer();

};
	)"""";
	std::string src_cmake_content = R""""(get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(targetname ${CURRENT_DIR_NAME})
add_executable(${targetname} main.cpp problem.cpp)
)"""";
	std::string tests_cmake_content = R""""(get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(targetname test_${CURRENT_DIR_NAME})
add_executable(${targetname} test.cpp ${PROJECT_SOURCE_DIR}/src/${CURRENT_DIR_NAME}/problem.cpp)
target_include_directories(${targetname} PUBLIC ${PROJECT_SOURCE_DIR}/googletest/googletest/include)
target_include_directories(${targetname} PUBLIC ${PROJECT_SOURCE_DIR}/src/${CURRENT_DIR_NAME}/)
target_link_libraries(${targetname} gtest_main)
)"""";
	std::string tests_cpp_content = R""""(#include "gtest/gtest.h"
#include "problem.h"

Solution solution;

int main(int argc, char *argv[])
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
)"""";
	// create files
	newfile(src + "/main.cpp", src_main_content);
	newfile(src + "/problem.cpp", src_problem_cpp_content);
	newfile(src + "/problem.h", src_problem_header_content);
	newfile(src + "/CMakeLists.txt", src_cmake_content);
	newfile(tests + "/CMakeLists.txt", tests_cmake_content);
	newfile(tests + "/test.cpp", tests_cpp_content);
	return 0;
}
}

#include "prt.h"
#include <fstream>

ProjectTemplate::ProjectTemplate()
{
	project_dir = std::filesystem::current_path();
	main_cmake_content = R""""(cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project_name VERSION 1.0)
include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(external)
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
set(EXENAME ${CURRENT_DIR_NAME})
add_executable(${EXENAME} main.cpp problem.cpp))"""";
	problem_test_cmake_content = R""""(if(BUILD_TESTING)
  get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  set(EXENAME ${DIR_NAME}_test)
  add_executable(${EXENAME} test.cpp
                            ${PROJECT_SOURCE_DIR}/src/${DIR_NAME}/problem.cpp)
  target_include_directories(${EXENAME}
                             PUBLIC ${PROJECT_SOURCE_DIR}/src/${DIR_NAME}/)
  target_link_libraries(${EXENAME} gtest_main)
  include(GoogleTest)
  gtest_discover_tests(${EXENAME})
endif())"""";
	main_cpp_content = R""""(#include "problem.h"
#include "timer.h"

int main(int argc, char *argv[])
{
	Solution solution;
	auto func = [&solution]()
	{
		solution.answer();
	};

	measure_exe_time(func);
	return 0;
})"""";
	problem_note_content = R""""(\section{标题}
\subsection{问题描述}
\begin{tcolorbox}

\end{tcolorbox}

\subsection{算法}
\begin{algorithm}
	\caption{算法标题}
	\begin{algorithmic}[1]

	\end{algorithmic}
\end{algorithm}

\subsection{答案})"""";

	time_t _tm = time(NULL);
	struct tm * curtime = localtime(&_tm);
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", curtime);
	std::string time_str(buffer);
	problem_cpp_content = R""""(/*
Problem page link:
https://projecteuler.net/problem=

dax )"""";
	problem_cpp_content += time_str + '\n';
	problem_cpp_content += R""""(*/
#include "problem.h"

void Solution::answer()
{
	std::cout << "The answer is: " << "" << std::endl;
})"""";
	problem_test_cpp_content = R""""(#include "gtest/gtest.h"
#include "problem.h"

Solution solution;

TEST(eulerno, testname)
{

}
)"""";
	problem_header_content = R""""(#pragma once
#include <iostream>

class Solution
{
	public:
		void answer();

};)"""";
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

void ProjectTemplate::create_project(std::string prj_name)
{

	// create dirs
	std::filesystem::path src_dir{project_dir}, test_dir{project_dir}, notes_dir{project_dir};

	src_dir /= "src";
	test_dir /= "tests";
	notes_dir /= "notes";

	if(!std::filesystem::is_directory(src_dir))
		mkdir(src_dir);

	if(!std::filesystem::is_directory(test_dir))
		mkdir(test_dir);

	if(!std::filesystem::is_directory(notes_dir))
		mkdir(notes_dir);

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
		create_file(main_cmake_path, main_cmake_content);

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
	int serial_no = std::stoi(prj_name.substr(7));
	auto n = problem_cpp_content.find('=');
	problem_cpp_content.insert(n + 1, std::to_string(serial_no));

	std::filesystem::path problem_cpp_path{problem_dir};
	problem_cpp_path /= "problem.cpp";

	if(!std::filesystem::is_regular_file(problem_cpp_path))
		create_file(problem_cpp_path, problem_cpp_content);

	// create main.cpp
	std::filesystem::path main_cpp_path{problem_dir};
	main_cpp_path /= "main.cpp";

	if(!std::filesystem::is_regular_file(main_cpp_path))
		create_file(main_cpp_path, main_cpp_content);

	// create problem test cpp file
	std::filesystem::path problem_test_cpp_path{problem_test_dir};
	problem_test_cpp_path /= "test.cpp";

	if(!std::filesystem::is_regular_file(problem_test_cpp_path))
		create_file(problem_test_cpp_path, problem_test_cpp_content);

	// create problem note
	std::filesystem::path problem_note_path{notes_dir};
	problem_note_path /= prj_name + ".tex";

	if(!std::filesystem::is_regular_file(problem_note_path))
		create_file(problem_note_path, problem_note_content);

	// add include problem to euler.tex
	std::filesystem::path euler_path{notes_dir};
	euler_path /= "euler.tex";
	std::fstream file(euler_path, std::ios::in | std::ios::out | std::ios::ate);
	std::string line;

	if(file.is_open())
	{
		auto pos = file.tellg();
		int count = 0;

		while(pos > 0)
		{
			pos -= static_cast<std::fstream::pos_type>(1);
			file.seekg(pos);

			if(file.get() == '\n')
			{
				++count;

				if(count == 2)
					break;

			}
		}

		file.seekp(pos);
		file << "\n\\include{" << prj_name << '}'
		     << "\n" << "\\end{document}" << std::endl;
	}

	file.close();

}

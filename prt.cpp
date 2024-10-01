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
	src_cmake_content = R""""(file(
  GLOB SUBDIRS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *)
foreach(SUBDIR ${SUBDIRS})
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
    # add_subdirectory(${SUBDIR})
    add_executable(${SUBDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/problem.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/main.cpp)
    # 将可执行文件放到一个单独的 bin 目录中
    set_target_properties(${SUBDIR} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                               ${CMAKE_BINARY_DIR}/bin)
    target_precompile_headers(${SUBDIR} PRIVATE
                              ${CMAKE_SOURCE_DIR}/include/iostream_wrapper.h)
    # 检查是否存在 config.cmake 文件
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/config.cmake)
      include(${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/config.cmake)
      # 如果 config.cmake 中定义了需要链接的库，就进行链接
      if(TARGET_LIBS)
        target_link_libraries(${SUBDIR} ${TARGET_LIBS})
      endif()
    endif()
  endif()
endforeach())"""";

	tests_cmake_content = R""""(file(
  GLOB SUBDIRS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  *)
foreach(SUBDIR ${SUBDIRS})
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
    # add_subdirectory(${SUBDIR})
    if(${SUBDIR} STREQUAL "lib")
      add_subdirectory(${SUBDIR})
    else()
      add_executable(
        ${SUBDIR}_test ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/test.cpp
                       ${CMAKE_SOURCE_DIR}/src/${SUBDIR}/problem.cpp)
      target_include_directories(${SUBDIR}_test
                                 PRIVATE ${CMAKE_SOURCE_DIR}/src/${SUBDIR})
      target_link_libraries(${SUBDIR}_test PRIVATE gtest_main)

      include(GoogleTest)
      gtest_discover_tests(${SUBDIR}_test)
      # 将可执行文件放到一个单独的 bin 目录中
      set_target_properties(${SUBDIR}_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      ${CMAKE_BINARY_DIR}/bin)
      target_precompile_headers(${SUBDIR}_test PRIVATE
                                ${CMAKE_SOURCE_DIR}/include/iostream_wrapper.h)
    endif()
    # 检查是否存在 config.cmake 文件
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/config.cmake)
      include(${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/config.cmake)
      # 如果 config.cmake 中定义了需要链接的库，就进行链接
      if(TARGET_LIBS)
        target_link_libraries(${SUBDIR}_test PRIVATE ${TARGET_LIBS})
      endif()
    endif()
  endif()
endforeach())"""";

	main_cpp_content = R""""(#include "problem.h"

int main(int argc, char *argv[])
{
	Solution solution;

	measure_exe_time(&Solution::answer, &solution);
	return 0;
})"""";
	problem_note_content = R""""(\section{标题}
\subsection{问题描述}
\begin{tcolorbox}

\end{tcolorbox}

\subsection{算法}


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
	print("The answer is:");
})"""";
	problem_test_cpp_content = R""""(#include "gtest/gtest.h"
#include "problem.h"

Solution solution;

TEST(eulerno, testname)
{

}
)"""";
	problem_header_content = R""""(#pragma once
#include <utils.h>

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

	// create src CMakeLists.txt
	std::filesystem::path src_cmake_path{src_dir};
	src_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(src_cmake_path))
		create_file(src_cmake_path, src_cmake_content);

	std::filesystem::path test_cmake_path{test_dir};
	test_cmake_path /= "CMakeLists.txt";

	if(!std::filesystem::is_regular_file(test_cmake_path))
		create_file(test_cmake_path, tests_cmake_content);

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

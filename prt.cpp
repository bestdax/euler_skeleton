#include "prt.h"
#include "template.h"

ProjectTemplate::ProjectTemplate()
{
	project_dir = std::filesystem::current_path();
	void load_templates();
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
	src_dir /= prj_name;
	test_dir /= "tests";
	test_dir /= prj_name;
	mkdir(src_dir);
	mkdir(test_dir);

	// create main project CMakeLists.txt
	std::filesystem::path main_cmake_path{project_dir};
	main_cmake_path /= "CMakeLists.txt";
	if(!std::filesystem::is_regular_file(main_cmake_path))
	{

	}
	
}

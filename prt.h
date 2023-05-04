#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>

class ProjectTemplate
{
public:
	ProjectTemplate();
	void create_euler_project(std::string prj_name);
	void create_algorith_project();
private:
	void mkdir(std::filesystem::path path);
	void create_file(std::filesystem::path path, std::string content);
	std::filesystem::path project_dir;
	void load_templates();
private:
	std::string main_cmake_content;
};

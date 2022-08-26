#include "Engine/Resources/FileSystem.h"

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace SC
{
	void FileSystem::CreateDirectory(const char* path)
	{
		std::filesystem::create_directory(path);
	}

	bool FileSystem::DirectoryExists(const char* path)
	{
		return std::filesystem::exists(path);
	}

	void FileSystem::CreateFile(const char* path)
	{	
		std::ofstream file(path);
		file.close();
	}

	bool FileSystem::FileExists(const char* path)
	{
		return std::filesystem::exists(path);
	}

	const char* FileSystem::ReadFile(const char* path)
	{
		std::ifstream file(path);
		char* data;

		file.read(data, std::filesystem::file_size(path));
		file.close();
		return data;
	}

	void FileSystem::WriteFile(const char* path, const char* data)
	{
		std::ofstream file(path);
		file << data;
		file.close();
	}
}

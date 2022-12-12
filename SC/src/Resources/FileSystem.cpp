#include "Engine/Resources/FileSystem.h"
#include "Engine/Core/Platform.h"

#include <cstdarg>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

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

	std::string FileSystem::ReadFile(std::string path)
	{
		std::ifstream file(path);
		std::string data; 
		file >> data;
		file.close();
		return data;
	}

	std::vector<std::filesystem::path> FileSystem::GetFilesInDirectory(const char* path, const char* ext)
	{
		std::vector<std::filesystem::path> strV;

		for (auto filepath : std::filesystem::recursive_directory_iterator(path)) {
			if (ext != nullptr)
			{
				if ((strcmp(filepath.path().extension().c_str(), ext) != 0) && ext != nullptr) continue;
			}
			strV.push_back(filepath);
		}

		return strV;
	}

	void FileSystem::WriteFile(const char* path, const char* data)
	{
		std::ofstream file(path);
		file << data;
		file.close();
	}

	std::string FileSystem::JoinPath(std::string a, std::string b)
	{
		return a + std::filesystem::path::preferred_separator + b;
	}
}

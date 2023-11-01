#include "Engine/Resources/FileSystem.h"
#include "Engine/Core/Platform.h"
#include "Engine/Debug/Debug.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <direct.h> // For _chdir on Windows
#define ChangeDirectory _chdir
#else
#include <unistd.h> // For chdir on Unix-like systems
#define ChangeDirectory chdir
#endif

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

	char* FileSystem::ReadFileBinary(std::string path, uint* size)
	{
		if (!FileSystem::FileExists(path.c_str())) {
			Debug::Error("File Not found - " + path, "FileSystem::FileNotFound");
		}

		uint filesize = std::filesystem::file_size(path);
		*size = filesize;
		
		std::ifstream file(path, std::ios::in | std::ios::binary);
		char* data = (char*)malloc(filesize);
		file.read(data, filesize);
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

	std::string FileSystem::RunProgram(std::string command, std::string args, std::string context, uint buffersize) {
		char* buffer = (char*)malloc(buffersize);
		std::string  output;

		if (context != "."s) {
			ChangeDirectory(context.c_str());
		}

		FILE* out;
		out = popen((command + " " + args).c_str(), "r");

		if (out == NULL) {
			Debug::Error("Couldn't run command: " + command + " " + args);
			return "ERROR"s;
		}

		while (fgets(buffer, buffersize, out) != NULL) {
        	output += buffer;
    	}

		return output;
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

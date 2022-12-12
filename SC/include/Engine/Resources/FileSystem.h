#pragma once

#include "Engine/Core/Core.h"
#include <filesystem>
#include <list>
#include <string>

namespace SC
{
	/**
	 * @brief A static class for File Handling with filesystem c++17 libary
	 * 
	 */
	class SC_API FileSystem
	{	
	public:
		/**
		 * @brief Create a Directory on specified path
		 * 
		 * @param path path that the directory will be created
		 */
		static void CreateDirectory(const char* path);
		/**
		 * @brief Checks if a directory exists or not
		 * 
		 * @param path path of directory
		 */
		static bool DirectoryExists(const char* path);

		/**
		 * @brief Create a File in given path
		 * 
		 * @param path path of file with its extention
		 */
		static void CreateFile(const char* path);
		/**
		 * @brief Checks if a file exists or not
		 * 
		 * @param path path of file
		 */
		static bool FileExists(const char* path);
		/**
		 * @brief Reads the File from path
		 * 
		 * @param path path of file
		 */
		static std::string ReadFile(std::string path);
		/**
		 * @brief Writes the data given to the given path
		 * 
		 * @param path The path of file
		 * @param data The data to be written
		 */
		static void WriteFile(const char* path, const char* data);

		/**
		 * @brief Joins the path given in a with b
		 * 
		 * @param a Base path
		 * @param b Path to join into @ref a "a"
		 * @return std::string 
		 */
		static std::string JoinPath(std::string a, std::string b);

		static std::vector<std::filesystem::path> GetFilesInDirectory(const char* path, const char* ext = nullptr);
	};
}

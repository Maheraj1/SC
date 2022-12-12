#include "Engine/Resources/Resources.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/FileSystem.h"
#include <filesystem>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <utility>

enum class ResourceType
{
	Texture,
	Shader
};

namespace SC
{
	template<typename T>
	requires (std::is_base_of_v<Resource, T>)
	std::unordered_map<std::string, T> ResourceMap<T>::data;

	template<typename T>
	requires (std::is_base_of_v<Resource, T>)
	void ResourceMap<T>::Clear()
	{
		for (auto res: ResourceMap<T>::data) {
			res.second.Delete();
		}
	}

	static const char Version[] = "0.0.1a";

	void Resources::LoadFileResources(const char* fp)
	{
		if (!FileSystem::FileExists(fp)) return;

		auto filepaths = FileSystem::GetFilesInDirectory(fp, ".scrd"); // .scrd means SC Resource Destination

		for (auto filepath : filepaths) {
			auto file = YAML::LoadFile(FileSystem::JoinPath(".", filepath));
			{
				std::string FileVersion = file["Version"].as<std::string>();

				if (FileVersion != Version)
				{
					Debug::Warning("Loaded resource file is older than current resource loader version (" + filepath.generic_string() + ')');
				}
			}

			ResourceType type = (ResourceType)file["Type"].as<int>();
			auto ResourceFile = filepath.replace_extension(file["Ext"].as<std::string>()).c_str();
			auto ResourceFileBaseName = filepath.stem().c_str();

			switch (type) {				
				case ResourceType::Texture: {
					Texture* tex = AddResource<Texture>(ResourceFileBaseName, ResourceFile);
					tex->uuid = file["UUID"].as<uint64_t>();
					Debug::Info("Loaded Texture");
					break;
				}
				case ResourceType::Shader: {
					AddResource<Shader>(ResourceFileBaseName, ResourceFile)->uuid = file["UUID"].as<uint64_t>();
					Debug::Info("Loaded Shader");
					break;
				}
			}
		}
	}
}

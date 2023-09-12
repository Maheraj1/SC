#include "Engine/Resources/Resources.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Serialization/SerializedData.h"
#include "yaml-cpp/node/parse.h"
#include <filesystem>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <utility>

enum class ResourceType
{
	Texture,
	Shader,
	Material
};

namespace SC
{
	template<typename T>
	requires (std::is_base_of_v<Resource, T>)
	std::vector<T> ResourceMap<T>::data;

	template<typename T>
	requires (std::is_base_of_v<Resource, T>)
	void ResourceMap<T>::Clear()
	{
		for (auto res: ResourceMap<T>::data) {
			res.Delete();
		}
	}

	static const char Version[] = "0.0.1a";

	void Resources::ReLoadFileResources(const char *fp) {
		if (!FileSystem::FileExists(fp)) return;

		ResourceMap<Texture>::Clear();
		ResourceMap<Shader>::Clear();
		ResourceMap<Material>::Clear();

		LoadFileResources(fp);
	}

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
					AddResource<Texture>(ResourceFileBaseName, ResourceFile)->uuid = file["UUID"].as<uint64_t>();
					Debug::Info("Loaded Texture");
					break;
				}
				case ResourceType::Shader: {
					AddResource<Shader>(ResourceFileBaseName, ResourceFile)->uuid = file["UUID"].as<uint64_t>();
					Debug::Info("Loaded Shader");
					break;
				}

				case ResourceType::Material: {
					
					auto mat = AddResource<Material>(ResourceFileBaseName, ResourceFile);
					mat->uuid = file["UUID"].as<uint64_t>();
					
					if (mat->IsSerializable()) {
						YAML::Node serialFile = YAML::Load(ResourceFile);
						Serialization::SerializedData::currentNode = &serialFile;
						mat->DeSerialize();
					}
					
					Debug::Info("Loaded Material");
					break;
				}
			}
		}
	}
}

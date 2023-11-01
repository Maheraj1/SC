#include "Engine/Resources/Resources.h"
#include "Engine/Core/Application.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Serialization/SerializedData.h"
#include "Engine/ECS/MonoCSScript.h"

#include "yaml-cpp/node/parse.h"
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <sstream>
#include <utility>

#define Basic_RES_SWITCH(name)\
case ResourceType::name:\
AddResource<name>(ResourceFileBaseName, ResourceFile.c_str())->uuid = uuid;\
Debug::Info("Loaded "s + #name);\
break;

#define Serializable_RES_SWITCH(name)\
case SerializableResourceType::name: {\
auto res = AddResource<name>(ResourceFileBaseName, ResourceFile);\
res->uuid = uuid;\
\
YAML::Node data;\
try { data = YAML::LoadFile(ResourceFile); } catch (std::exception e)\
{ Debug::Error("Failed to load " + (std::string)ResourceFile + " Resource",\
 "SC::Resources::LoadSerializableFileResources::"s + #name); }\
YAML::Node serialFile = data;\
Serialization::SerializedData::currentNode = &serialFile;\
res->DeSerialize();}\
Debug::Info("Loaded "s + #name);\
Serialization::SerializedData::currentNode = nullptr;\
break;\


enum class ResourceType
{
	Texture,
	Shader,
};

enum class SerializableResourceType
{
	General,
	Material
};

namespace SC
{
	bool Resources::autoSave = true;

	static const char Version[] = "0.0.1a";

	void Resources::ReLoadFileResources(std::string fp) {
		if (!FileSystem::FileExists(fp.c_str())) return;

		ResourceMap<Texture>::Clear();
		ResourceMap<Shader>::Clear();
		ResourceMap<Material>::Clear();

		LoadFileResources(fp);
	}

	void Resources::LoadFileResources(std::string fp)
	{
		if (!FileSystem::FileExists(fp.c_str())) return;

		auto filepaths = FileSystem::GetFilesInDirectory(fp.c_str(), ".scrd"); // .scrd means SC Resource Destination

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
			std::string ResourceFile = filepath.replace_extension(file["Ext"].as<std::string>()).c_str();
			std::string ResourceFileBaseName = filepath.stem().c_str();
			UUID uuid = file["UUID"].as<uint64_t>();

			switch (type) {
				Basic_RES_SWITCH(Texture) 
				Basic_RES_SWITCH(Shader)
				default:break;
			}
		}
	}

	void Resources::LoadSerializableFileResources(std::string fp)
	{
		if (!FileSystem::FileExists(fp.c_str())) return;

		// .scsrd means SC Serializable Resource Destination
		auto filepaths = FileSystem::GetFilesInDirectory(fp.c_str(), ".scsrd");

		for (auto filepath : filepaths) {
			auto file = YAML::LoadFile(FileSystem::JoinPath(".", filepath));
			{
				std::string FileVersion = file["Version"].as<std::string>();

				if (FileVersion != Version)
				{
					Debug::Warning("Loaded resource file is older than current resource loader version (" + filepath.generic_string() + ')');
				}
			}

			SerializableResourceType type = (SerializableResourceType)file["Type"].as<int>();
			std::string ResourceFile = filepath.replace_extension(file["Ext"].as<std::string>()).c_str();
			std::string ResourceFileBaseName = filepath.stem().c_str();
			UUID uuid = file["UUID"].as<uint64_t>();

			switch (type) {

				Serializable_RES_SWITCH(Material)

				default:
					break;
			}
		}
	}


	void SaveSerializableResource(SerializableResource* serializableResource) {
		Application::Get()->AddEndOfFrameFunction([&](){
			serializableResource->Serialize();
		});
	}
}

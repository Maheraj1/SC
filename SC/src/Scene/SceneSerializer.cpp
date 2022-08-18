#include "Engine/Scene/SceneSerializer.h"
#include "Engine/ECS/Script.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Scene/SceneManager.h"

#include "Engine/Serialization/SerializedData.h"

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <string>

namespace SC
{
	YAML::Emitter* SceneSerializer::emt = nullptr;
	static const char version[8] = "0.0.1b\0";

	SceneSerializer::SceneSerializer() {
		
	}

	void SceneSerializer::Init()
	{
		emt = new YAML::Emitter;
		Serialization::SerializedData::emt = emt;
	}

	void SceneSerializer::SerializeText(const std::string filePath)
	{
		emt = new YAML::Emitter;
		Serialization::SerializedData::emt = emt;
		*emt << YAML::BeginMap;
		*emt << YAML::Key << "Version" << YAML::Value << version;
		*emt << YAML::Key << "Components" << YAML::Value << YAML::Flow << YAML::BeginSeq;
		for (auto& com : Internal::ComponentData::components) {
			*emt << com;
		}
		*emt << YAML::EndSeq;
		
		Scene& scene = SceneManager::GetCurrentScene();

		*emt << YAML::Key << "ComponentsRegistered" << YAML::Value << Internal::ComponentData::components.size();
		
		*emt << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;


		for (int i = 0; i < scene.m_objs.size(); i++)
		{
			*emt << YAML::BeginMap;
			*emt << YAML::Key << "GUID" << YAML::Value << scene.m_objs[i].GetUUID();
			*emt << YAML::Key << "Name" << scene.m_objs[i].name;
			*emt << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;
			scene.m_objs[i].transform._Serialize();
			*emt << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

			for (int i2 = 0; i2 < scene.m_objs[i].components.size(); i2++) {
				*emt << YAML::BeginMap;
				*emt << YAML::Key << "Name" << scene.m_objs[i].components[i2]->name;
				scene.m_objs[i].components[i2]->_Serialize();
			}
			*emt << YAML::EndSeq;
			
			*emt << YAML::EndMap;
		}

		*emt << YAML::EndSeq;
		*emt << YAML::EndMap;

		std::ofstream f(filePath);
		f << emt->c_str();
		
		delete emt;
	}

	bool IsInComponents(const char* name)
	{
		for (const char* str: Internal::ComponentData::components)
			if (strcmp(name, str) == 0) 
				return true;

		return false;
	}

	bool SceneSerializer::DeserializeText(const std::string filePath, void(*func)(const char*, Entity*))
	{
		Physics::ShutDown();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath);
		} catch (YAML::ParserException e)
		{
			Debug::Error("Failed to load " + filePath + " scene", "SC::SceneSerializer::DeserializeText");
			return false;
		}
		if (!data["Version"]) return false;
		Debug::Info((std::string)"Current Scene Loader Version " + version + " Scene file version " + data["Version"].as<std::string>(), "SC::SceneSerializer::DeserializeText");
		
		auto dat = data["Objects"];
		if (dat)
		{
			int comC = data["ComponentsRegistered"].as<int>();
			Scene& scene = SceneManager::AddScene();

			auto com = data["Components"];
			if (!data["Components"].IsSequence()) return false;

			for (auto ent: dat)
			{
				UUID uid = ent["GUID"].as<uint64_t>();
				std::string name = ent["Name"].as<std::string>();

				auto& _ent = scene.AddEntity(name, uid);
				auto trans = ent["Transform"];
				Serialization::SerializedData::currentNode = &trans;
				
				for (int i = 0; i < ent["Components"].size(); i++)
				{
					auto string = ent["Components"][i]["Name"].as<std::string>();
					const char* name = string.c_str();
					if (IsInComponents(name))
					{
						auto scr = ent["Components"][i];
						Serialization::SerializedData::currentNode = &scr;
						func(name, &_ent);
						_ent.components[i]->_DeSerialize();
					}
				}
			}
		}
		Physics::Init();

		return true;
	}
}

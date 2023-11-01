#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/IScript.h"
#include "Engine/ECS/MonoCSScript.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Scene/SceneManager.h"

#include "Engine/Serialization/SerializedData.h"

#include "yaml-cpp/emittermanip.h"
#include "yaml-cpp/yaml.h"

#include <exception>
#include <fstream>
#include <string>
#include <vector>

namespace SC
{
	YAML::Emitter* SceneSerializer::emt = nullptr;
	static const char version[8] = "0.0.5a\0";

	SceneSerializer::SceneSerializer() {
		
	}

	void SceneSerializer::Init()
	{
		emt = new YAML::Emitter;
		Serialization::SerializedData::emt = emt;
	}

	void SceneSerializer::SerializeText(const Scene& scene) {
		SerializeText(scene, scene.FilePath);
	}

	bool SceneSerializer::DeserializeText(Scene& scene) {
		return DeserializeText(scene, scene.FilePath);
	}

	void SceneSerializer::SerializeText(const Scene& scene, std::string path)
	{
		emt = new YAML::Emitter;
		Serialization::SerializedData::emt = emt;
		*emt << YAML::BeginMap;
		*emt << YAML::Key << "Version" << YAML::Value << version;
		
		*emt << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;


		for (int i = 0; i < scene.m_objs.size(); i++)
		{
			*emt << YAML::BeginMap;
			*emt << YAML::Key << "Name" << YAML::Value << scene.m_objs[i]->name;
			*emt << YAML::Key << "UUID" << YAML::Value << scene.m_objs[i]->GetUUID();
			*emt << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;
			scene.m_objs[i]->transform._Serialize();
			*emt << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

			for (int i2 = 0; i2 < scene.m_objs[i]->scripts.size(); i2++) {
				*emt << YAML::BeginMap;
				*emt << YAML::Key << "IsNative" << YAML::TrueFalseBool << scene.m_objs[i]->scripts[i2]->IsNative();
				*emt << YAML::Key;

				Internal::Component& comp = Internal::ComponentData::components[scene.m_objs[i]->scripts[i2]->GetCID()];

				*emt << YAML::Key << "Name" << YAML::Value << comp.qualifiedName;
				scene.m_objs[i]->scripts[i2]->_Serialize();
			}
			*emt << YAML::EndSeq;
			
			*emt << YAML::EndMap;
		}

		*emt << YAML::EndSeq;
		*emt << YAML::EndMap;

		auto fp = std::filesystem::path(path).parent_path();
		
		if (!FileSystem::DirectoryExists(fp.c_str())) {
			FileSystem::CreateDirectory(fp.c_str());
		}

		FileSystem::WriteFile(path.c_str(), emt->c_str());
		
		delete emt;
	}

	bool SceneSerializer::DeserializeText(Scene& scene, std::string path)
	{
		int del_size = scene.m_objs.size();
		for (int i = 0; i < del_size; i++) {
			delete scene.m_objs[0];
			scene.m_objs.erase(scene.m_objs.begin());
		}
		Physics::ShutDown();

		YAML::Node data;
		try {
			data = YAML::LoadFile(path);
		} catch (std::exception e) {
			Debug::Error("Failed to load " + (std::string)path + " scene", "SC::SceneSerializer::DeserializeText");
			return false;
		}
		
		// Version shows validity of file
		if (!data["Version"]) return false;

		Debug::Info((std::string)"Current Scene Loader Version " + version + " Scene file version " + data["Version"].as<std::string>(), "SC::SceneSerializer::DeserializeText");
		
		auto dat = data["Objects"];
		if (dat) {
			for (int i = 0; i < dat.size(); i++) {
				auto ent = dat[i];
				std::string name = ent["Name"].as<std::string>();
				UUID uid = ent["UUID"].as<uint64_t>();

				auto& _ent = scene.AddEntity(name, uid);
				auto trans = ent["Transform"];
				Serialization::SerializedData::currentNode = &trans;
				_ent.transform._DeSerialize();
				
				for (int i = 0; i < ent["Components"].size(); i++) {
					auto scr = ent["Components"][i];
					auto scrN = scr["Name"].as<std::string>();
					Serialization::SerializedData::currentNode = &scr;
					
					IScript* script = _ent.AddComponent(Internal::ComponentData::QualifiedNameToCID[scrN]+1);

					if (!scr["IsNative"].as<bool>()) {
						((MonoCSScript*)script)->Init(scr["MonoName"].as<std::string>());
					}

					_ent.scripts[i]->_DeSerialize();
				}
			}
		}
		Physics::Init();

		return true;
	}
}

#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/SceneManager.h"

#include <fstream>

namespace SC
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer() {}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << entity.name;
		out << YAML::Value << entity.m_id;

		out << YAML::Key << typeid(entity.transform).name();
		out << YAML::BeginMap;

		out << YAML::Key << "Position";
		out << YAML::BeginMap;
		out << "X" << YAML::Value << entity.transform.position.x;
		out << "Y" << YAML::Value << entity.transform.position.y;
		out << YAML::EndMap;

		out << YAML::Key << "Rotation" << YAML::Value << entity.transform.rotation;

		out << YAML::Key << "Scale";
		out << YAML::BeginMap;
		out << "X" << YAML::Value << entity.transform.scale.x;
		out << "Y" << YAML::Value << entity.transform.scale.y;

		out << YAML::EndMap;

		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const std::string filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;

		Scene& scene = SceneManager::GetCurrentScene();

		for (int i = scene.m_objs.capacity() - 1; i >= 0; i++)
		{
			SerializeEntity(out, scene.m_objs[i]);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream f(filePath);
		f << out.c_str();
	}
}

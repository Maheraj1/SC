#pragma once

#include "../Core/Core.h"
#include "Scene.h"

#include "yaml-cpp/yaml.h"

#include <iostream>

namespace SC::Internal::Components {
	inline void AddComponent(const char* str, SC::Entity* ent);
}

namespace SC
{
	class SceneSerializer
	{
	private:
		static YAML::Emitter* emt;
	public:
		SceneSerializer();

		static void SerializeText(const std::string filePath);
		static void SerializeBinary(const std::string filePath);

		static bool DeserializeText(const std::string filePath, void(*func)(const char*, Entity*) = Internal::Components::AddComponent);
		static bool DeserializeBinary(const std::string filePath);

		static void Init();

		friend class Serialization::SerializedData;
	};
}
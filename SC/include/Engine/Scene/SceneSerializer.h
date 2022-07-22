#pragma once

#include "../Core/Core.h"
#include "Scene.h"

#include "yaml-cpp/yaml.h"

#include <iostream>

namespace SC
{
	class SceneSerializer
	{
	public:
		SceneSerializer();

		static void SerializeText(const std::string filePath);
		static void SerializeBinary(const std::string filePath);

		static bool DeserializeText(const std::string filePath);
		static bool DeserializeBinary(const std::string filePath);
	protected:
		static void SerializeEntity(YAML::Emitter& out, Entity& entity);
	};
}

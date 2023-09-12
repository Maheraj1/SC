#pragma once

#include "../Core/Core.h"
#include "Scene.h"

#include "yaml-cpp/yaml.h"

#include <functional>
#include <iostream>
#include <unordered_map>

#if defined(SC_CORE_IMPL) || defined(SC_EDITOR_IMPL)
	#ifdef SC_DEBUG
		#define SC_Serialize(x)				  	 SC::SceneSerializer::SerializeText  (x)
		#define SC_DeSerialize(x)			  	 SC::SceneSerializer::DeserializeText(x)
		#define SC_SerializeOtherPath(x, path)   SC::SceneSerializer::SerializeText  (x, path)
		#define SC_DeSerializeOtherPath(x, path) SC::SceneSerializer::DeserializeText(x, path)
	#else
		#define SC_Serialize  (x) SC::SceneSerializer::SerializeBinary  (x)
		#define SC_DeSerialize(x) SC::SceneSerializer::DeserializeBinary(x)
	#endif
#endif

namespace SC
{
	class SC_API SceneSerializer
	{
	private:
		static YAML::Emitter* emt;
	public:
		SceneSerializer();

		static void SerializeText  (const Scene& scene, std::string path);
		static void SerializeText  (const Scene& scene);
		static void SerializeBinary(const Scene& scene, std::string path);
		static void SerializeBinary(const Scene& scene);

		static bool DeserializeText  (Scene& scene, std::string path);
		static bool DeserializeText  (Scene& scene);
		static bool DeserializeBinary(Scene& scene, std::string path);
		static bool DeserializeBinary(Scene& scene);

		static void Init();

		friend class Serialization::SerializedData;
	};
}
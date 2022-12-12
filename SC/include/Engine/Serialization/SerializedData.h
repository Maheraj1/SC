#pragma once

#include "Engine/Math/Math.h"

#include <string>

#define SC_ADD_PARAM SC::Serialization::SerializedData::AddValue
#define SC_GET_PARAM SC::Serialization::SerializedData::GetValue

#define SC_ADD_PARAMETER(x) SC::Serialization::SerializedData::AddValue(x, #x)
#define SC_GET_PARAMETER(x) SC::Serialization::SerializedData::GetValue(x, #x)

#define SC_ADD_PARAMETER_S(x, y) SC::Serialization::SerializedData::AddValue(x, y)
#define SC_GET_PARAMETER_S(x, y) SC::Serialization::SerializedData::GetValue(x, y)

#define SerializableObj Serialization::SerializableObject

namespace YAML { class Emitter; class Node; }

namespace SC {
	class SceneSerializer;
}

namespace SC::Serialization {
	namespace Internal {
		std::string CapitalizeWord(std::string name);
	}
	class SerializableObject;
	
	class SC_API SerializedData
	{
	private:
		static YAML::Emitter* emt;
		static YAML::Node* currentNode;
	public:
		// Serialize
		template<typename T>
		static void AddValue(const T& dat, const char* name);

		template<typename T>
		static void AddValue(const T* dat, const char* name);

		// DeSerialize

		template<typename T>
		static void GetValue(T& dat, const char* name);

		template<typename T>
		static void GetValue(T* dat, const char* name);

		// Helper functions
		static void* GetEmitter();
		static void* GetCurrent();

		friend class SC::SceneSerializer;
	};
}

#pragma once

#include "Engine/Core/Buffer.h"
#include "Engine/Math/Math.h"

#include <cstddef>
#include <string>
#include <sys/_types/_u_char.h>

#define SC_ADD_PARAM SC::Serialization::SerializedData::AddValue
#define SC_GET_PARAM SC::Serialization::SerializedData::GetValue

#define SC_ADD_PARAM_RW SC::Serialization::SerializedData::AddValueRaw
#define SC_GET_PARAM_RW SC::Serialization::SerializedData::GetValueRaw

#define SC_ADD_PARAMETER_RW(dat, size) SC::Serialization::SerializedData::AddValueRaw(dat, size, #dat)
#define SC_GET_PARAMETER_RW(dat, size) SC::Serialization::SerializedData::GetValueRaw(dat, size, #dat)

#define SC_ADD_PARAMETER(x) SC::Serialization::SerializedData::AddValue(x, #x)
#define SC_GET_PARAMETER(x) SC::Serialization::SerializedData::GetValue(x, #x)

#define SC_ADD_PARAMETER_S(x, y) SC::Serialization::SerializedData::AddValue(x, y)
#define SC_GET_PARAMETER_S(x, y) SC::Serialization::SerializedData::GetValue(x, y)

#define SerializableObj Serialization::SerializableObject

namespace YAML { class Emitter; class Node; }

namespace SC {
	class SceneSerializer;
	class Resources;
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

		static void AddValueRaw(const Buffer& buf, const char* name);

		// DeSerialize

		template<typename T>
		static void GetValue(T& dat, const char* name);

		template<typename T>
		static void GetValue(T* dat, const char* name);

		static void GetValueRaw(Buffer& buf, const char* name);
		
		// Helper functions
		static void* GetEmitter();
		static void* GetCurrent();

		static void SetNewEmitter();
		static void SaveEmitter(std::string fp);

		friend class SC::SceneSerializer;
		friend class SC::Resources;
	};
}

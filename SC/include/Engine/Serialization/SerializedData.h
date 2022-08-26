#pragma once

#include "Engine/Core/Math.h"

#include <string>

#define SC_ADD_PARAMETER(x) SC::Serialization::SerializedData::AddValue(x, SC::Serialization::Internal::CapitalizeWord(#x).c_str())
#define SC_GET_PARAMETER(x) SC::Serialization::SerializedData::GetValue(x, SC::Serialization::Internal::CapitalizeWord(#x).c_str())


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
		static void AddValue(int dat, const char* name);
		static void AddValue(unsigned int dat, const char* name);
		
		static void AddValue(int64_t dat, const char* name);
		static void AddValue(uint64_t dat, const char* name);

		static void AddValue(int16_t dat, const char* name);
		static void AddValue(uint16_t dat, const char* name);
		
		static void AddValue(signed char dat, const char* name);
		static void AddValue(unsigned char dat, const char* name);

		static void AddValue(bool dat, const char* name);

		static void AddValue(float dat, const char* name);
		static void AddValue(double dat, const char* name);

		static void AddValue(const std::string& dat, const char* name);
		static void AddValue(const char* dat, const char* name);

		static void AddValue(const Vector2f& dat, const char* name);
		static void AddValue(const Vector2i& dat, const char* name);

		static void AddValue(const Vector3f& dat, const char* name);
		static void AddValue(const Vector3i& dat, const char* name);
		
		static void AddValue(const Vector4f& dat, const char* name);
		static void AddValue(const Vector4i& dat, const char* name);

		static void AddValue(const Color& dat, const char* name);
		static void AddValue(const Color16& dat, const char* name);

		static void AddValue(const SerializableObject* dat, const char* name);

		// DeSerialize

		static void GetValue(int& dat, const char* name);
		static void GetValue(unsigned int& dat, const char* name);
		
		static void GetValue(int64_t& dat, const char* name);
		static void GetValue(uint64_t& dat, const char* name);

		static void GetValue(int16_t& dat, const char* name);
		static void GetValue(uint16_t& dat, const char* name);
		
		static void GetValue(signed char& dat, const char* name);
		static void GetValue(unsigned char& dat, const char* name);

		static void GetValue(bool& dat, const char* name);

		static void GetValue(float& dat, const char* name);
		static void GetValue(double& dat, const char* name);

		static void GetValue(std::string& dat, const char* name);

		static void GetValue(Vector2f& dat, const char* name);
		static void GetValue(Vector2i& dat, const char* name);

		static void GetValue(Vector3f& dat, const char* name);
		static void GetValue(Vector3i& dat, const char* name);
		
		static void GetValue(Vector4f& dat, const char* name);
		static void GetValue(Vector4i& dat, const char* name);

		static void GetValue(Color& dat, const char* name);
		static void GetValue(Color16& dat, const char* name);

		static void GetValue(SerializableObject& dat, const char* name);

		static void* GetEmitter();
		static void* GetCurrent();

		friend class SC::SceneSerializer;
	};
}

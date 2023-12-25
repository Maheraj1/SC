#include "Engine/Serialization/SerializedData.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Math/Math.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Scripting/SerializableField.h"
#include "Engine/Serialization/SerializableObject.h"

#include "yaml-cpp/binary.h"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/emittermanip.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <sys/_types/_u_char.h>

#define NAME__NULL if (name == nullptr) throw std::runtime_error("name of parameter is required");

namespace YAML {

	template<>
	struct convert<glm::fvec2>
	{
		static Node encode(const glm::fvec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::fvec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::ivec2>
	{
		static Node encode(const glm::ivec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::ivec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<SC::Vector3i>
	{
		static Node encode(const SC::Vector3i& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, SC::Vector3i& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec<3, short>>
	{
		static Node encode(const glm::vec<3, short>& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec<3, short>& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<short>();
			rhs.y = node[1].as<short>();
			rhs.z = node[2].as<short>();
			return true;
		}
	};
}

namespace SC::Serialization 
{
	YAML::Emitter* SerializedData::emt = nullptr;
	YAML::Node* SerializedData::currentNode = nullptr;

	void* SerializedData::GetEmitter   () { return emt; }
	void SerializedData::SetNewEmitter () { emt = new YAML::Emitter; }
	
	void SerializedData::SaveEmitter(std::string fp) { FileSystem::WriteFile(fp.c_str(), emt->c_str()); }

	template<>
	void SerializedData::AddValue<int>(const int& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<unsigned int>(const unsigned int& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	
	template<>
	void SerializedData::AddValue<int64_t>(const int64_t& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<uint64_t>(const uint64_t& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}


	template<>
	void SerializedData::AddValue<int16_t>(const int16_t& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<uint16_t>(const uint16_t& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<float>(const float& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}
		
	template<>
	void SerializedData::AddValue<double>(const double& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<long double>(const long double& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt.WriteStreamable(dat);
	}
	
	template<>
	void SerializedData::AddValue<signed char>(const signed char& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<unsigned char>(const unsigned char& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<bool>(const bool& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<std::string>(const std::string& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValueRaw(const Buffer& buf, const char* name)
	{
		NAME__NULL
		
		YAML::Binary bin((u_char*)buf.Data, buf.Size);
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << bin;
	}


	template<>
	void SerializedData::AddValue<Vector2f>(const Vector2f& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector2i>(const Vector2i& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector3f>(const Vector3f& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector3i>(const Vector3i& dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue(const SerializableResource* dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << YAML::BeginMap;
		dat->Serialize();
	}

	template<>
	void SerializedData::AddValue(const SerializableObject* dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << YAML::BeginMap;
		dat->_Serialize();
	}

	template<>
	void SerializedData::AddValue(const std::vector<SerializableObject*>* dat, const char* name)
	{
		NAME__NULL
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << YAML::BeginSeq << YAML::Flow;
		
		for (int i = 0; i < dat->size(); i++) {
			_emt << YAML::BeginMap;
			dat->at(i)->_Serialize();
		}
	}

	template<>
	void SerializedData::GetValue<>(int& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<int>();
	}
	template<>
	void SerializedData::GetValue<>(unsigned int& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<unsigned int>();
	}
	
	template<>
	void SerializedData::GetValue<>(int64_t& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<int64_t>();
	}
	template<>
	void SerializedData::GetValue<>(uint64_t& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<uint64_t>();
	}

	template<>
	void SerializedData::GetValue<>(int16_t& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<int16_t>();
	}
	template<>
	void SerializedData::GetValue<>(uint16_t& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<uint16_t>();
	}
	
	template<>
	void SerializedData::GetValue<>(signed char& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<signed char>();
	}
	template<>
	void SerializedData::GetValue<>(unsigned char& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<unsigned char>();
	}

	template<>
	void SerializedData::GetValue<>(bool& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<bool>();
	}

	template<>
	void SerializedData::GetValue<>(float& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<float>();
	}
	template<>
	void SerializedData::GetValue<>(double& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<double>();
	}

	template<>
	void SerializedData::GetValue<>(std::string& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<std::string>();
	}

	void SerializedData::GetValueRaw(Buffer& buf, const char* name)
	{
		NAME__NULL
		
		auto bin = (*currentNode)[name].as<YAML::Binary>();
		buf = Buffer(bin.size());
		buf.Data = (uint8_t*)bin.data();
	}

	template<>
	void SerializedData::GetValue<>(Vector2f& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<Vector2f>();
	}
	template<>
	void SerializedData::GetValue<>(Vector2i& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<Vector2i>();
	}

	template<>
	void SerializedData::GetValue<>(Vector3f& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<Vector3f>();
	}
	template<>
	void SerializedData::GetValue<>(Vector3i& dat, const char* name)
	{
		NAME__NULL
		dat = (*currentNode)[name].as<Vector3i>();
	}

	template<>
	void SerializedData::GetValue<>(SerializableResource* dat, const char* name)
	{
		NAME__NULL
		auto nodeBackup = currentNode;
		auto node =(*currentNode)[name];
		currentNode = &node;
		dat->DeSerialize();
		currentNode = nodeBackup;
	}

	template<>
	void SerializedData::GetValue<>(SerializableObject* dat, const char* name)
	{
		NAME__NULL
		auto nodeBackup = currentNode;
		auto node =(*currentNode)[name];
		currentNode = &node;
		dat->_DeSerialize();
		currentNode = nodeBackup;
	}

	template<>
	void SerializedData::GetValue<>(std::vector<SerializableObject*>* dat, const char* name)
	{
		NAME__NULL
		auto nodeBackup = currentNode;
		auto node =(*currentNode)[name];
		currentNode = &node;

		auto listNode = currentNode;
		
		for (int i = 0; i < listNode->size(); i++) {
			currentNode = &listNode[i];
			dat->at(i)->_DeSerialize();
		}
		
		currentNode = nodeBackup;
	}

	template<>
	void SerializedData::GetValue<>(std::vector<Scripting::SerializableField*>* dat, const char* name)
	{
		NAME__NULL
		auto nodeBackup = currentNode;
		auto node =(*currentNode)[name];
		currentNode = &node;

		auto listNode = currentNode;

		dat->reserve(listNode->size());
		
		for (int i = 0; i < listNode->size(); i++) {
			currentNode = &listNode[i];
			(*dat)[i] = new Scripting::SerializableField;
			(*dat)[i]->DeSerialize();
		}
		
		currentNode = nodeBackup;
	}
}
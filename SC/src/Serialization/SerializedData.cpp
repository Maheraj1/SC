#include "Engine/Serialization/SerializedData.h"
#include "Engine/Math/Math.h"
#include "Engine/Serialization/SerializableObject.h"

#include "yaml-cpp/yaml.h"
#include <cctype>
#include <stdexcept>

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

	template<>
	struct convert<SC::Color>
	{
		static Node encode(const SC::Color& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, SC::Color& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<uint8_t>();
			rhs.y = node[1].as<uint8_t>();
			rhs.z = node[2].as<uint8_t>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::ivec4>
	{
		static Node encode(const glm::ivec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::ivec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			rhs.w = node[3].as<int>();
			return true;
		}
	};
}

namespace SC::Serialization 
{
	YAML::Emitter* SerializedData::emt = nullptr;
	YAML::Node* SerializedData::currentNode = nullptr;

	void* SerializedData::GetEmitter() { return emt; }

	template<>
	void SerializedData::AddValue<int>(const int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<unsigned int>(const unsigned int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	
	template<>
	void SerializedData::AddValue<int64_t>(const int64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<uint64_t>(const uint64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}


	template<>
	void SerializedData::AddValue<int16_t>(const int16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<uint16_t>(const uint16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<float>(const float& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}
		
	template<>
	void SerializedData::AddValue<double>(const double& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<long double>(const long double& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt.WriteStreamable(dat);
	}
	
	template<>
	void SerializedData::AddValue<signed char>(const signed char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<unsigned char>(const unsigned char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<bool>(const bool& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<>(const std::string& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	template<>
	void SerializedData::AddValue<const char*>(const char* const & dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}


	template<>
	void SerializedData::AddValue<Vector2f>(const Vector2f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector2i>(const Vector2i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}


	template<>
	void SerializedData::AddValue<Vector3f>(const Vector3f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector3i>(const Vector3i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	
	template<>
	void SerializedData::AddValue<Vector4f>(const Vector4f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << dat.w << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Vector4i>(const Vector4i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << dat.w << YAML::EndSeq;
	}


	template<>
	void SerializedData::AddValue<Color>(const Color& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.r << dat.g << dat.b << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue<Color16>(const Color16& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.r << dat.g << dat.b << YAML::EndSeq;
	}

	template<>
	void SerializedData::AddValue(const SerializableObject* dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << YAML::BeginMap;
		dat->_Serialize();
		// _emt << YAML::EndMap;
	}

	template<>
	void SerializedData::GetValue<>(int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int>();
	}
	template<>
	void SerializedData::GetValue<>(unsigned int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<unsigned int>();
	}
	
	template<>
	void SerializedData::GetValue<>(int64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int64_t>();
	}
	template<>
	void SerializedData::GetValue<>(uint64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<uint64_t>();
	}

	template<>
	void SerializedData::GetValue<>(int16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int16_t>();
	}
	template<>
	void SerializedData::GetValue<>(uint16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<uint16_t>();
	}
	
	template<>
	void SerializedData::GetValue<>(signed char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<signed char>();
	}
	template<>
	void SerializedData::GetValue<>(unsigned char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<unsigned char>();
	}

	template<>
	void SerializedData::GetValue<>(bool& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<bool>();
	}

	template<>
	void SerializedData::GetValue<>(float& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<float>();
	}
	template<>
	void SerializedData::GetValue<>(double& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<double>();
	}

	template<>
	void SerializedData::GetValue<>(std::string& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<std::string>();
	}

	template<>
	void SerializedData::GetValue<>(Vector2f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector2f>();
	}
	template<>
	void SerializedData::GetValue<>(Vector2i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector2i>();
	}

	template<>
	void SerializedData::GetValue<>(Vector3f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector3f>();
	}
	template<>
	void SerializedData::GetValue<>(Vector3i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector3i>();
	}
	
	template<>
	void SerializedData::GetValue<>(Vector4f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector4f>();
	}
	template<>
	void SerializedData::GetValue<>(Vector4i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector4i>();
	}

	template<>
	void SerializedData::GetValue<>(Color& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Color>();
	}
	template<>
	void SerializedData::GetValue<>(Color16& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Color16>();
	}

	template<>
	void SerializedData::GetValue<SerializableObject>(SerializableObject* dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		auto nodeBackup = currentNode;
		auto node =(*currentNode)[name];
		currentNode = &node;
		dat->_DeSerialize();
		currentNode = nodeBackup;
	}

}
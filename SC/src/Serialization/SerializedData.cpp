#include "Engine/Serialization/SerializedData.h"
#include "Engine/Core/Math.h"
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

	std::string Internal::CapitalizeWord(std::string name)
	{
		name[0] = toupper(name[0]);
		return name;
	}

	void* SerializedData::GetEmitter() { return emt; }

	void SerializedData::AddValue(int dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(unsigned int dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	
	void SerializedData::AddValue(int64_t dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(uint64_t dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}


	void SerializedData::AddValue(int16_t dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(uint16_t dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(float dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}
		
	void SerializedData::AddValue(double dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}
	
	void SerializedData::AddValue(signed char dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(unsigned char dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(bool dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(const std::string& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}

	void SerializedData::AddValue(const char* dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value << dat;
	}


	void SerializedData::AddValue(const Vector2f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}

	void SerializedData::AddValue(const Vector2i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << YAML::EndSeq;
	}


	void SerializedData::AddValue(const Vector3f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	void SerializedData::AddValue(const Vector3i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << YAML::EndSeq;
	}

	
	void SerializedData::AddValue(const Vector4f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << dat.w << YAML::EndSeq;
	}

	void SerializedData::AddValue(const Vector4i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.x << dat.y << dat.z << dat.w << YAML::EndSeq;
	}


	void SerializedData::AddValue(const Color& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.r << dat.g << dat.b << YAML::EndSeq;
	}

	void SerializedData::AddValue(const Color16& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		_emt << YAML::Flow << YAML::BeginSeq << dat.r << dat.g << dat.b << YAML::EndSeq;
	}


	void SerializedData::AddValue(const SerializableObject* dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		
		YAML::Emitter& _emt = *emt;
		_emt << YAML::Key << name << YAML::Value;
		dat->_Serialize();
	}

	void SerializedData::GetValue(int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int>();
	}
	void SerializedData::GetValue(unsigned int& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<unsigned int>();
	}
	
	void SerializedData::GetValue(int64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int64_t>();
	}
	void SerializedData::GetValue(uint64_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<uint64_t>();
	}

	void SerializedData::GetValue(int16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<int16_t>();
	}
	void SerializedData::GetValue(uint16_t& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<uint16_t>();
	}
	
	void SerializedData::GetValue(signed char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<signed char>();
	}
	void SerializedData::GetValue(unsigned char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<unsigned char>();
	}

	void SerializedData::GetValue(bool& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<bool>();
	}

	void SerializedData::GetValue(float& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<float>();
	}
	void SerializedData::GetValue(double& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<double>();
	}

	void SerializedData::GetValue(std::string& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<std::string>();
	}
	void SerializedData::GetValue(char& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<char>();
	}

	void SerializedData::GetValue(Vector2f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector2f>();
	}
	void SerializedData::GetValue(Vector2i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector2i>();
	}

	void SerializedData::GetValue(Vector3f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector3f>();
	}
	void SerializedData::GetValue(Vector3i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector3i>();
	}
	
	void SerializedData::GetValue(Vector4f& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector4f>();
	}
	void SerializedData::GetValue(Vector4i& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Vector4i>();
	}

	void SerializedData::GetValue(Color& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Color>();
	}
	void SerializedData::GetValue(Color16& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required");
		dat = (*currentNode)[name].as<Color16>();
	}

	void SerializedData::GetValue(SerializableObject& dat, const char* name)
	{
		if (name == nullptr) throw std::runtime_error("name of parameter is required and method not implemented");
		// *dat = (*currentNode)[name].as<SerializableObject>();
	}

}
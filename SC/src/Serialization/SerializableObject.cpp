#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Core/SCObject.h"
#include "Engine/Serialization/SerializedData.h"

#include "yaml-cpp/yaml.h"

namespace SC::Serialization {
	
	SerializableObject::SerializableObject() { }

	void SerializableObject::_Serialize() const
	{
		YAML::Emitter& emt = *((YAML::Emitter*)SerializedData::GetEmitter());
		Serialize();
		emt << YAML::EndMap;
	}

	void SerializableObject::_DeSerialize()
	{
		DeSerialize();
	}
}

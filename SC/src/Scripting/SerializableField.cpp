#include "Engine/Scripting/SerializableField.h"
#include "Engine/Serialization/SerializedData.h"
#include <malloc/_malloc.h>
#include <sys/_types/_u_char.h>

namespace SC::Scripting {
	void SerializableField::Serialize() const {
		SC_ADD_PARAM<std::string>(name, "Name");
		SC_ADD_PARAM<std::string>(type, "Type");
		SC_ADD_PARAM_RW(data, "Data");
	}
	
	void SerializableField::DeSerialize() {
		SC_GET_PARAM<std::string>(name, "Name");
		SC_GET_PARAM<std::string>(type, "Type");
		SC_GET_PARAM_RW(data, "Data");
	}
}
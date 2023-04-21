#include "Engine/Serialization/Serialization.h"
#include "Engine/ECS/IScript.h"
#include <vector>
#include <unordered_map>

namespace SC::Serialization {
	std::string Serialization::GetComponentNameByTypeName(std::string name) {
		return ::SC::Internal::ComponentData::components.at(::SC::Internal::ComponentData::TypeNameToCID[name]).qualifiedName;
	}

	Serialization::CreateFunc Serialization::GetComponentCreateFuncByTypeName(std::string name) {
		return ::SC::Internal::ComponentData::components.at(::SC::Internal::ComponentData::TypeNameToCID[name]).CreateFunc;
	}
}

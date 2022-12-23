#include "Engine/Serialization/Serialization.h"
#include "Engine/ECS/ComponentData.h"
#include <vector>
#include <unordered_map>

namespace SC::Serialization {
	std::string Serialization::GetComponentNameByTypeName(std::string name) {
		return ::SC::Internal::ComponentData::components.at(::SC::Internal::ComponentData::TypeNameToComponentName[name]);
	}

	Serialization::CreateFunc Serialization::GetComponentCreateFuncByTypeName(std::string name) {
		return ::SC::Internal::ComponentData::NameToFunc.at(GetComponentNameByTypeName(name));
	}
}

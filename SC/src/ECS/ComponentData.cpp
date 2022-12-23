#include "Engine/ECS/ComponentData.h"
#include <vector>

namespace SC::Internal {
	std::vector<const char*> ComponentData::components;
	std::unordered_map<std::string, int> ComponentData::TypeNameToComponentName;
	std::unordered_map<std::string, void(*)(Entity*)> ComponentData::NameToFunc;
}

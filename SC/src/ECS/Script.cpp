#include "Engine/ECS/Script.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Serialization/SerializedData.h"
#include <array>
#include <unordered_map>

namespace SC {
	Script::Script() { }

	void Script::Destroy(Entity* ent)
	{
		SceneManager::GetCurrentScene().DestroyEntity(ent);
	}

	namespace Internal {
        std::vector<const char*> ComponentData::components;
		std::unordered_map<const char*, int> ComponentData::NameToComponents;
		std::unordered_map<std::string, void(*)(Entity*)> ComponentData::NameToFunc;
    }
}
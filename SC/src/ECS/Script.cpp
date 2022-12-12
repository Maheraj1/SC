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
		std::unordered_map<std::string, int> ComponentData::TypeNameToComponentName;
		std::unordered_map<std::string, void(*)(Entity*)> ComponentData::NameToFunc;
    }

	void Script::Serial() const
	{
		_Serialize();
	}

	void Script::DeSerial()
	{
		_DeSerialize();
	}
}
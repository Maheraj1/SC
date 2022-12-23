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

	void Script::Serial() const
	{
		_Serialize();
	}

	void Script::DeSerial()
	{
		_DeSerialize();
	}
}
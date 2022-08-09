#include "Engine/ECS/Script.h"
#include "Engine/Scene/SceneManager.h"
#include <array>

namespace SC {
	Script::Script() { }

	void Script::Destroy(Entity* ent)
	{
		SceneManager::GetCurrentScene().DestroyEntity(ent);
	}
}
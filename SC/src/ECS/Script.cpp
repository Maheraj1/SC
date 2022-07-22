#include "Engine/ECS/Script.h"
#include "Engine/Scene/SceneManager.h"
#include <array>

namespace SC {
	std::array<std::function<void(void)>, 4> Script::GetFunctions()
	{
		std::array<std::function<void(void)>, 4> funcs = {
			[&](){Start();}, [&](){Update();}, [&](){Awake();}, [&](){OnDestroy();}
		};
		return funcs;
	}

	Script::Script() { }

	void Script::Destroy(Entity* ent)
	{
		SceneManager::GetCurrentScene().DestroyEntity(ent);
	}
}
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Core/Base.h"

namespace SC 
{
	int SceneManager::CurrentScene = 0;
	std::vector<Scene> SceneManager::scenes;

	SceneManager::SceneManager() { }

	bool SceneManager::LoadScene(int index)
	{
		// SceneSerializer::Deserialize();
		return false;
	}

	Scene& SceneManager::AddScene()
	{
		// create a Scene Object at index of value of scene.size()
		scenes.push_back(Scene());
		CurrentScene = scenes.size() - 1;
		return scenes.back();
	}

	int SceneManager::GetCurrentSceneIndex()
	{
		return CurrentScene;
	}

	Scene& SceneManager::GetCurrentScene()
	{
		// if (SceneCount > CurrentScene)
		return scenes[CurrentScene];
		// else
		// 	SC_DEBUGBREAK();
	}	
}

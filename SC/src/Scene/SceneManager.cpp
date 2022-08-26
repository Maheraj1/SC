#include "Engine/Scene/SceneManager.h"
#include "Engine/Core/Errors.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Core/Base.h"

namespace SC 
{
	int SceneManager::CurrentSceneIndex = 0;
	std::vector<const char*> SceneManager::scenes;
	std::vector<Scene> SceneManager::LoadedScenes;

	SceneManager::SceneManager() { }

	bool SceneManager::LoadScene(int index)
	{
		if (scenes.size() < index) return false;
		SC_Serialize(scenes.at(index));

		return true;
	}

	Scene& SceneManager::AddScene(const char* fp)
	{
		LoadedScenes.emplace_back(fp);
		CurrentSceneIndex = LoadedScenes.size() - 1;
		return LoadedScenes.back();
	}

	void SceneManager::RemoveScene()
	{
		LoadedScenes.erase(LoadedScenes.end());
	}

	void SceneManager::RemoveSceneUsingIndex(int index)
	{
		LoadedScenes.erase(LoadedScenes.begin() + index);
	}

	void SceneManager::SaveScene(int index)
	{
		
	}

	void SceneManager::SaveScene(Scene& scene)
	{
		
	}

	int SceneManager::GetCurrentSceneIndex()
	{
		return CurrentSceneIndex;
	}

	Scene& SceneManager::GetScene(int index)
	{
		return LoadedScenes[index];
	}

	Scene& SceneManager::GetCurrentScene()
	{
		return LoadedScenes[0];
	}

	Scene* SceneManager::GetCurrentScenes()
	{
		return &LoadedScenes[0];
	}	
}

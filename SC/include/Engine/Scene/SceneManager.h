#pragma once

#include "Engine/Scene/SceneSerializer.h"
#include "Scene.h"

#include <vector>

namespace SC {
	class SceneManager
	{
		public:
			static std::vector<const char*> scenes;
			static std::vector<Scene> LoadedScenes;
			static int CurrentSceneIndex;
		public:
			static bool LoadScene(int index);
			static int GetCurrentSceneIndex();

			static Scene& AddScene(const char* fp = nullptr);

			static void RemoveScene();
			static void RemoveSceneUsingIndex(int index);

			static void SaveScene(int index);
			static void SaveScene(Scene& scene);

			static Scene& GetCurrentScene();
			static Scene* GetCurrentScenes();

			static Scene& GetScene(int index);
		private:
			SceneManager();
		
		friend class Internal::Renderer;
	};
}

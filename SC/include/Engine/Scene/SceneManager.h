#pragma once

#include "Scene.h"

#include <vector>

namespace SC {
	class SceneManager
	{
		public:
			static std::vector<Scene> scenes;
			static int CurrentScene;
		public:
			static bool LoadScene(int index);
			static int GetCurrentSceneIndex();
			static Scene& AddScene();
			static Scene& GetCurrentScene();
		private:
			SceneManager();
		
		friend class Internal::Renderer;
	};
}

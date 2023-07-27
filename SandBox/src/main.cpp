#include <Engine/SC.h>

void PreAppRun()
{
	Application::AutoGenerateTexture = true;
	Scene& scene = SceneManager::AddScene("test.yaml");
	scene.Load();
}
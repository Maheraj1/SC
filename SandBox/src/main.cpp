#include "Engine/Core/Application.h"
#include "Engine/Core/Math.h"
#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Input/KeyCode.h"
#include "Engine/Resources/Resources.h"
#include <Engine/SC.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/ECS/Script.h>
#include <string>

using namespace SC;

class CameraMovement: public Script
{
	private:
		void Start() { }

		void Update()
		{
			Vector2f move;
			if (Input::GetKey(KeyCode::A)) entity->transform.position.x += 1.0f * Time::deltaTime;
			if (Input::GetKey(KeyCode::D)) entity->transform.position.x -= 1.0f * Time::deltaTime;
			
			if (Input::GetKey(KeyCode::S)) entity->transform.position.y += 1.0f * Time::deltaTime;
			if (Input::GetKey(KeyCode::W)) entity->transform.position.y -= 1.0f * Time::deltaTime;
		}
	friend class Component<CameraMovement>;
};

class SpriteMovement: public Script
{
	private:
		void Start()  { }
		void Update() { }
	friend class Component<SpriteMovement>;
};


void PreAppRun()
{
	Application::AutoGenerateTexture = true;
 	Scene& scene = SceneManager::AddScene();
	
	Entity& ent = scene.AddEntity("Test");
	ent.AddComponent<SpriteRenderer>().texture = Resources::AddTexturePtr("tex", "Square0.png");
	ent.AddComponent<SpriteMovement>();

	// ent.transform.scale = Vector2(10, 10);

	Entity& cam = scene.AddEntity("Camera");
	cam.AddComponent<Camera>();
	cam.AddComponent<CameraMovement>();
}

int main()
{
	RunApp();
}

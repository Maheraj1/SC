#include "Editor/EditorAddon.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Core/Platform.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Scene/SceneManager.h"
#include <Engine/SC.h>

using namespace SC;

class CameraMovement: public Script
{
	public:
		float speed = 2.0f;
		int i = 0;
	private:
		Camera* cam;
		Vector2f move = {0, 0};

		void Start() { 
			cam  = GetComponentPtr<Camera>();
			move = entity->transform.position;
		}

		void Update()
		{

			if (Input::GetKey(KeyCode::A)) move.x += 1.0f * speed * Time::deltaTime;
			if (Input::GetKey(KeyCode::D)) move.x -= 1.0f * speed * Time::deltaTime;
			
			if (Input::GetKey(KeyCode::S)) move.y += 1.0f * speed * Time::deltaTime;
			if (Input::GetKey(KeyCode::W)) move.y -= 1.0f * speed * Time::deltaTime;

			if (Input::GetKeyDown(KeyCode::Q)) cam->size += 1;
			if (Input::GetKeyDown(KeyCode::E)) cam->size -= 1;

			entity->transform.position = Math::Lerp(entity->transform.position, move, Time::deltaTime * 2.0f);
		}

		virtual void Serialize() const override { 
			SC_ADD_PARAMETER(speed);
		}

		virtual void DeSerialize() override
		{
			SC_GET_PARAMETER(speed);
		}
	friend class Component<CameraMovement>;
};

class SpriteMovement: public Script
{
	private:
		void Start()  {
			
		 }
		void Update() { 
			if (Input::GetKey(KeyCode::G)) entity->transform.position.x += 1.0f * Time::deltaTime;
			if (Input::GetKey(KeyCode::J)) entity->transform.position.x -= 1.0f * Time::deltaTime;
			
			if (Input::GetKey(KeyCode::H)) entity->transform.position.y += 1.0f * Time::deltaTime;
			if (Input::GetKey(KeyCode::Y)) entity->transform.position.y -= 1.0f * Time::deltaTime;
		}
	friend class Component<SpriteMovement>;
};

void PreAppRun()
{
	SC_REGISTER_COMPONENT(CameraMovement);
	SC_REGISTER_COMPONENT(SpriteMovement);

	Scene& scene = SceneManager::AddScene("Assets/test.scs");
	scene.Load();
	Application::Get()->addons.push_back(new Editor::EditorAddon);
}
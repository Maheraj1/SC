#include "Editor/EditorAddon.h"
#include "Engine/Core/EntryPoint.h"
#include "Engine/Core/Platform.h"
#include "Engine/ECS/Camera.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scripting/RegisteredScript.h"
#include <Engine/SC.h>

using namespace SC;

//* DEPRECATED: Will be moved to C#
// class CameraMovement: public NativeScript
// {
// 	public:
// 		float speed = 2.0f;
// 		int i = 0;
// 	private:
// 		Camera* cam;
// 		Vector2f move = {0, 0};

// 		void Start() { 
// 			cam  = (Camera*)entity->GetComponent(ComponentID<Camera>::cid);
// 			move = entity->transform.position;
// 		}

// 		void Update()
// 		{

// 			if (Input::GetKey(KeyCode::A)) move.x += 1.0f * speed * Time::deltaTime;
// 			if (Input::GetKey(KeyCode::D)) move.x -= 1.0f * speed * Time::deltaTime;
			
// 			if (Input::GetKey(KeyCode::S)) move.y += 1.0f * speed * Time::deltaTime;
// 			if (Input::GetKey(KeyCode::W)) move.y -= 1.0f * speed * Time::deltaTime;

// 			if (Input::GetKeyDown(KeyCode::Q)) cam->size += 1;
// 			if (Input::GetKeyDown(KeyCode::E)) cam->size -= 1;

// 			entity->transform.position = Math::Lerp(entity->transform.position, move, Time::deltaTime * 2.0f);
// 		}

// 		virtual void Serialize() const override { 
// 			SC_ADD_PARAMETER(speed);
// 		}

// 		virtual void DeSerialize() override
// 		{
// 			SC_GET_PARAMETER(speed);
// 		}

// 		GET_CID_IMPL_SRC(CameraMovement);
// };

void AddApplicationAddons() {
	Application::Get()->addons.push_back(new Editor::EditorAddon);
}

void PreAppRun()
{
	Scene& scene = SceneManager::AddScene("Assets/Scene/test.scs");
	
	{
		Entity& cam = scene.AddEntity("Camera");
		cam.AddComponent(ComponentID<Camera>::cid);
	}

	{
		Entity& box = scene.AddEntity("Box");
		box.transform.position.y = 5;
		
		RigidBody* rb = (RigidBody*)box.AddComponent(ComponentID<RigidBody>::cid);
		rb->mass = 2.0f;
		rb->type = RigidBodyType::Dynamic;

		box.AddComponent(ComponentID<BoxCollider>::cid);
		
		SpriteRenderer* sr = (SpriteRenderer*)box.AddComponent(ComponentID<SpriteRenderer>::cid);
		sr->material.obj = Resources::GetResource<Material>("DefaultMaterial");
		sr->color = Color(128, 128, 128);
	}

	{
		Entity& floor = scene.AddEntity("Floor");
		floor.transform.position.y = -5;
		floor.transform.scale.x = 5;
		
		floor.AddComponent(ComponentID<RigidBody>::cid);
		floor.AddComponent(ComponentID<BoxCollider>::cid);
		
		SpriteRenderer* sr = (SpriteRenderer*)floor.AddComponent(ComponentID<SpriteRenderer>::cid);
		sr->material.obj = Resources::GetResource<Material>("DefaultMaterial");
		sr->color = Color(255, 0, 0);
	}

	scene.Save();

	// scene.Load();
}
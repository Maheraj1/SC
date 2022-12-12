#include <Engine/SC.h>

#include "Engine/Core/Application.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Math.h"

#include "Engine/Debug/Debug.h"
#include <Engine/Debug/Timmer.h>

#include "Engine/ECS/Component.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/ECS/Camera.h"
#include <Engine/ECS/Script.h>

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCode.h"

#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/RigidBody.h"

#include "Engine/Resources/Resources.h"

#include "Engine/Scene/SceneSerializer.h"
#include <Engine/Scene/SceneManager.h>

#include "Engine/Serialization/SerializedData.h"

#include <string>

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

			if (Input::GetKeyDown(KeyCode::Z)) cam->size += 1;
			if (Input::GetKeyDown(KeyCode::X)) cam->size -= 1;

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
	Application::AutoGenerateTexture = true;
	
	SC_REGISTER_COMPONENT(CameraMovement);
	SC_REGISTER_COMPONENT(SpriteMovement);
	Scene& scene = SceneManager::AddScene("test.yaml");
	scene.Load();
	
	// {
	// 	Entity& ent = scene.AddEntity("Test");
	// 	// ent.AddComponent<SpriteRenderer>().texture = Resources::AddTexture("tex", "Square0.png");
	// 	ent.transform.position = { 2.0f, 5.0f };
	// 	auto& rb = ent.AddComponent<RigidBody>();
	// 	rb.type = RigidBodyType::Dynamic;
	// 	rb.mass = 1.0f;

	// 	auto& coll = ent.AddComponent<BoxCollider>();
	// }

	// {
	// 	Entity& ent = scene.AddEntity("Ground");
	// 	// ent.AddComponent<SpriteRenderer>().texture = Resources::GetTexture("tex");
	// 	ent.AddComponent<RigidBody>();
	// 	ent.AddComponent<BoxCollider>().size = Vector2f(5.0f, 1.0f);
	// 	ent.transform.scale = Vector2f(5.0f, 1.0f);
	// 	ent.transform.position.y = -5;
	// }

	// Entity& cam = scene.AddEntity("Camera");
	// cam.AddComponent<Camera>();
	// cam.AddComponent<CameraMovement>().i = 1;

	// scene.Save();
}
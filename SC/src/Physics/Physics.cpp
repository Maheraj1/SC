#include "Engine/Physics/Physics.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Time.h"
#include "Engine/Physics/RigidBody.h"
#include "box2d/b2_body.h"
#include "box2d/b2_draw.h"
#include "box2d/b2_math.h"
#include "box2d/b2_world.h"
#include "box2d/box2d.h"
#include <vector>
#include "Engine/Scene/SceneManager.h"
#include "glm/trigonometric.hpp"

namespace SC {
	// Class static variables
	Vector2f Physics::gravity = Vector2f(0.0f, -10.0f);
	std::vector<RigidBody*> Physics::rigidBodies;
	b2World* Physics::world = nullptr;

	void Physics::Init()
	{
		if (world != nullptr) delete world;
		world = new b2World(ToBox2dVector2(Physics::gravity));
	}

	void Physics::Step(float dt)
	{
		world->Step(dt, 6, 2);
	}

	void Physics::ShutDown() { 
		if (world == nullptr) return;
		delete world;
		world = nullptr;
	}

	void Physics::UpdateData()
	{
		for (RigidBody* rb: rigidBodies) rb->ApplyParameters();
	}

	void Physics::UpdatePositions()
	{
		static uint64_t& rb_cid = ComponentID<RigidBody>::cid;

		auto& objs = SceneManager::GetCurrentScene().m_objs;

		for (int i = 0; i < objs.size(); i++) {
			RigidBody* rb = (RigidBody*)objs[i]->GetComponent(rb_cid);
			if (rb == nullptr) continue;
			objs[i]->transform.position = FromBox2dVector2(rb->body->GetPosition());
			objs[i]->transform.rotation = glm::degrees(rb->body->GetAngle());
		}
            
	}

	b2World* Physics::GetPhysicsWorld()
	{
		return world;
	}
}

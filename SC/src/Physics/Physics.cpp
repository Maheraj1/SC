#include "Engine/Physics/Physics.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Time.h"
#include "Engine/Physics/RigidBody.h"
#include "box2d/box2d.h"
#include <vector>
#include "Engine/Scene/SceneManager.h"
#include "glm/trigonometric.hpp"

namespace SC {
	// Class static variables
	Vector2f Physics::gravity = Vector2f(0.0f, -10.0f);
	std::vector<RigidBody*> Physics::rigidBodies;
	b2WorldId Physics::world = b2_nullWorldId;

	void Physics::Init()
	{
		if (b2World_IsValid(world)) b2DestroyWorld(world);
		b2WorldDef def = b2DefaultWorldDef();
		def.gravity = ToBox2dVector2(Physics::gravity);
		world = b2CreateWorld(&def);
	}

	void Physics::Step(float dt)
	{
		b2World_Step(world, dt, 6);
	}

	void Physics::ShutDown() { 
		if (!b2World_IsValid(world)) return;
		b2DestroyWorld(world);
		world = b2_nullWorldId;
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
			b2Pos pos = b2Body_GetPosition(rb->body);
			objs[i]->transform.position = Vector2f((float)pos.x, (float)pos.y);
			b2Rot rot = b2Body_GetRotation(rb->body);
			objs[i]->transform.rotation = glm::degrees(b2Rot_GetAngle(rot));
		}
            
	}

	b2WorldId Physics::GetPhysicsWorld()
	{
		return world;
	}
}

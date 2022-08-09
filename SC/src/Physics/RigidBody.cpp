#include "Engine/Physics/RigidBody.h"
#include "Engine/Core/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Physics/Physics.h"
#include "b2_body.h"
#include "b2_fixture.h"
#include "b2_math.h"
#include "b2_shape.h"
#include "box2d.h"

namespace SC {

	void RigidBody::Awake()
	{
		b2BodyDef def;
		Vector2f pos = entity->transform.position;
		def.position.Set(pos.x, pos.y);
		def.angle = glm::radians(entity->transform.rotation);
		def.type = (b2BodyType)type;

		body = Physics::world->CreateBody(&def);
		// b2MassData MDat;
		// MDat.mass = mass;
		// MDat.center.Set(pos.x, pos.y);
		// body->SetMassData(&MDat);

		body->SetTransform(ToBox2dVector2(entity->transform.position), glm::radians(entity->transform.rotation));
		physicsID = Physics::rigidBodies.size();
		Physics::rigidBodies.push_back(this);
	}

	RigidBody::RigidBody() { }

	RigidBody::~RigidBody() { }

	void RigidBody::OnDestroy()
	{
		Physics::world->DestroyBody(body);
		Physics::rigidBodies.erase(Physics::rigidBodies.begin() + physicsID);
	}

	void RigidBody::FixedUpdate()
	{
		Vector2f pos = entity->transform.position;
		body->SetTransform(ToBox2dVector2(pos), glm::radians(entity->transform.rotation));
	}

	void RigidBody::ApplyParameters()
	{
		Vector2f pos = entity->transform.position;
		b2MassData MDat;
		MDat.mass = mass;
		MDat.center.Set(pos.x, pos.y);
		MDat.I = 0;
		body->SetMassData(&MDat);
		body->SetTransform(ToBox2dVector2(pos), entity->transform.rotation);
		
		OnApplyParameters({});
	}
}
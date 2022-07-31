#include "Engine/Physics/RigidBody.h"
#include "Engine/Core/Math.h"
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

		body = Physics::GetWorld().CreateBody(&def);
		b2MassData MDat;
		MDat.mass = mass;
		MDat.center.Set(pos.x, pos.y);
		body->SetMassData(&MDat);
	}

	RigidBody::RigidBody() { }

	RigidBody::~RigidBody() { }

	void RigidBody::OnDestroy()
	{
		Physics::GetWorld().DestroyBody(body);
	}

	void RigidBody::FixedUpdate()
	{
		Vector2f pos = entity->transform.position;
		body->SetTransform(ToBox2dVector2(pos), glm::radians(entity->transform.rotation));
		b2MassData MDat;
		MDat.mass = mass;
		MDat.center.Set(pos.x, pos.y);
		body->SetMassData(&MDat);
	}
}
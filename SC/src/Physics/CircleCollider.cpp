#include "Engine/Physics/CircleCollider.h"
#include "Engine/Core/Event.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "b2_collision.h"
#include "b2_circle_shape.h"
#include "b2_shape.h"
#include "b2_world.h"
#include "b2_fixture.h"
#include "b2_body.h"

namespace SC {
	void CircleCollider::Start()
	{
		rb = TryGetComponent<RigidBody>();
		if (rb == nullptr) return;

		Vector2f pos = entity->transform.position;
		shape = new b2CircleShape();
		shape->m_radius = size;

		b2FixtureDef def;
		def.density = Density;
		def.friction = Friction;
		def.restitution = Restitution;
		def.restitutionThreshold = RestitutionThreshold;
		def.isSensor = IsTrigger;
		def.shape = shape;
		fixture = rb->body->CreateFixture(&def);
		rb->OnApplyParameters += [&](EventArgs e) {ApplyParameters();};
	}

	void CircleCollider::ApplyParameters()
	{
		fixture->SetDensity(Density);
		fixture->SetFriction(Friction);
		fixture->SetRestitution(Restitution);
		fixture->SetRestitutionThreshold(RestitutionThreshold);
		fixture->SetSensor(IsTrigger);
		shape->m_radius = size;
	}
}
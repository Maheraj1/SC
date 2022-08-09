#include "Engine/Physics/BoxCollider.h"
#include "Engine/Core/Event.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "b2_polygon_shape.h"
#include "b2_world.h"
#include "b2_fixture.h"
#include "b2_body.h"

namespace SC {
	void BoxCollider::Start()
	{
		rb = TryGetComponent<RigidBody>();
		if (rb == nullptr) return;

		Vector2f pos = entity->transform.position;
		shape = new b2PolygonShape();
		shape->SetAsBox(size.x, size.y);

		b2FixtureDef def;
		def.density = Density;
		def.friction = Friction;
		def.restitution = Restitution;
		def.restitutionThreshold = RestitutionThreshold;
		def.isSensor = IsTrigger;
		def.shape = shape;
		fixture = rb->body->CreateFixture(&def);
		rb->OnApplyParameters += [&](EventArgs e){ApplyParameters();};
	}

	void BoxCollider::ApplyParameters()
	{
		fixture->SetDensity(Density);
		fixture->SetFriction(Friction);
		fixture->SetRestitution(Restitution);
		fixture->SetRestitutionThreshold(RestitutionThreshold);
		fixture->SetSensor(IsTrigger);
		shape->SetAsBox(size.x, size.y);
	}
}
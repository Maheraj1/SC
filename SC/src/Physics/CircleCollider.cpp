#include "Engine/Physics/CircleCollider.h"
#include "Engine/Core/Event.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "box2d/box2d.h"

namespace SC {
	void CircleCollider::Start()
	{
		rb = (RigidBody*)entity->GetComponent(ComponentID<RigidBody>::cid);
		if (rb == nullptr) return;

		b2ShapeDef def = b2DefaultShapeDef();
		def.density = Density;
		def.material.friction = Friction;
		def.material.restitution = Restitution;
		def.isSensor = IsTrigger;

		b2Circle circ = {{0.0f, 0.0f}, size};
		circle = circ;
		fixture = b2CreateCircleShape(rb->body, &def, &circle);
		rb->OnApplyParameters += [&](EventArgs e) {ApplyParameters();};
	}

	void CircleCollider::ApplyParameters()
	{
		b2Shape_SetDensity(fixture, Density, true);
		b2Shape_SetFriction(fixture, Friction);
		b2Shape_SetRestitution(fixture, Restitution);

		b2Circle circ = {{0.0f, 0.0f}, size};
		circle = circ;
		b2Shape_SetCircle(fixture, &circle);
	}

	void CircleCollider::Serialize() const
	{
		SC_ADD_PARAMETER(Density);
		SC_ADD_PARAMETER(Friction);
		SC_ADD_PARAMETER(Restitution);
		SC_ADD_PARAMETER(RestitutionThreshold);
		SC_ADD_PARAMETER(IsTrigger);
		SC_ADD_PARAMETER(size);
	}

	void CircleCollider::DeSerialize()
	{
		SC_GET_PARAMETER(Density);
		SC_GET_PARAMETER(Friction);
		SC_GET_PARAMETER(Restitution);
		SC_GET_PARAMETER(RestitutionThreshold);
		SC_GET_PARAMETER(IsTrigger);
		SC_GET_PARAMETER(size);
	}

	GET_CID_IMPL(CircleCollider);
}
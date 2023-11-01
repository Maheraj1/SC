#include "Engine/Physics/BoxCollider.h"
#include "Engine/Core/Event.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_world.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_body.h"

namespace SC {
	void BoxCollider::Start()
	{
		rb = (RigidBody*)entity->GetComponent(ComponentID<RigidBody>::cid);
		if (rb == nullptr) return;

		// Vector2f pos = entity->transform.position;
		shape = new b2PolygonShape();
		shape->SetAsBox(size.x+offset.x, size.y+offset.y);

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
		shape->SetAsBox(size.x+offset.x, size.y+offset.y);
	}

	void BoxCollider::Serialize() const
	{
		SC_ADD_PARAMETER(Density);
		SC_ADD_PARAMETER(Friction);
		SC_ADD_PARAMETER(Restitution);
		SC_ADD_PARAMETER(RestitutionThreshold);
		SC_ADD_PARAMETER(IsTrigger);
		SC_ADD_PARAMETER(size);
		SC_ADD_PARAMETER(offset);
	}

	void BoxCollider::DeSerialize()
	{
		SC_GET_PARAMETER(Density);
		SC_GET_PARAMETER(Friction);
		SC_GET_PARAMETER(Restitution);
		SC_GET_PARAMETER(RestitutionThreshold);
		SC_GET_PARAMETER(IsTrigger);
		SC_GET_PARAMETER(size);
		SC_GET_PARAMETER(offset);
	}

	#ifdef SC_EDITOR_IMPL
	void BoxCollider::OnIGUI(Editor::EditorDrawData& dcmd) {
		dcmd.DrawFloat  (Friction, "Friction");
		dcmd.DrawFloat  (Density, "Density");
		dcmd.DrawFloat  (Restitution, "Bounciness");
		dcmd.DrawFloat  (RestitutionThreshold, "Bounciness Threshold");
		dcmd.DrawVector2(size, "Size");
		dcmd.DrawVector2(offset, "Offset");
		dcmd.DrawBool  (IsTrigger, "Is Trigger");
	}

	void BoxCollider::PostIGUI(Editor::EditorDrawData& dcmd) {
		Friction = *((float*)dcmd.data[0].data);
		Density = *((float*)dcmd.data[1].data);
		Restitution = *((float*)dcmd.data[2].data);
		RestitutionThreshold = *((float*)dcmd.data[3].data);
		size = *((Vector2*)dcmd.data[4].data);
		offset = *((Vector2*)dcmd.data[5].data);
		IsTrigger = *((float*)dcmd.data[6].data);
	}

	#endif

	
	GET_CID_IMPL(BoxCollider);
}
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Core/Event.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "box2d/box2d.h"

namespace SC {
	void BoxCollider::Start()
	{
		rb = (RigidBody*)entity->GetComponent(ComponentID<RigidBody>::cid);
		if (rb == nullptr) return;

		b2ShapeDef def = b2DefaultShapeDef();
		def.density = Density;
		def.material.friction = Friction;
		def.material.restitution = Restitution;
		def.isSensor = IsTrigger;

		b2Polygon poly = b2MakeBox(size.x + offset.x, size.y + offset.y);
		shape = poly;
		fixture = b2CreatePolygonShape(rb->body, &def, &shape);
		rb->OnApplyParameters += [&](EventArgs e){ApplyParameters();};
	}

	void BoxCollider::ApplyParameters()
	{
		b2Shape_SetDensity(fixture, Density, true);
		b2Shape_SetFriction(fixture, Friction);
		b2Shape_SetRestitution(fixture, Restitution);

		b2Polygon poly = b2MakeBox(size.x + offset.x, size.y + offset.y);
		shape = poly;
		b2Shape_SetPolygon(fixture, &shape);
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
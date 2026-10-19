#include "Engine/Physics/RigidBody.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Serialization/SerializedData.h"
#include "box2d/box2d.h"

namespace SC {

	void RigidBody::Awake()
	{
		b2BodyDef def = b2DefaultBodyDef();
		Vector2f pos = entity->transform.position;
		def.position = b2Vec2(pos.x, pos.y);
		def.rotation = b2MakeRot(glm::radians(entity->transform.rotation));
		def.type = (b2BodyType)type;

		body = b2CreateBody(Physics::world, &def);

		b2Body_SetTransform(body, b2Vec2(entity->transform.position.x, entity->transform.position.y), b2MakeRot(glm::radians(entity->transform.rotation)));
		physicsID = Physics::rigidBodies.size();
		Physics::rigidBodies.push_back(this);
	}

	RigidBody::RigidBody() { }

	RigidBody::~RigidBody() {
		if (!b2Body_IsValid(body)) return;
		b2DestroyBody(body);
		int rmid = -1;
		
		for (int i = 0; Physics::rigidBodies.size(); i++) {
			if (Physics::rigidBodies[i] != this) continue; 
			
			rmid = i;
			break;
		}

		Physics::rigidBodies.erase(Physics::rigidBodies.begin() + rmid);
		body = b2_nullBodyId;
	}

	void RigidBody::OnDestroy() {

	}

	void RigidBody::FixedUpdate()
	{
		// Vector2f pos = entity->transform.position;
		// body->SetTransform(ToBox2dVector2(pos), glm::radians(entity->transform.rotation));
	}

	void RigidBody::ApplyParameters()
	{
		Vector2f pos = entity->transform.position;
		b2Body_SetTransform(body, b2Vec2(pos.x, pos.y), b2MakeRot(glm::radians(entity->transform.rotation)));
		
		OnApplyParameters({});
	}

	void RigidBody::Serialize() const
	{
		SC_ADD_PARAMETER(mass);
		int type = (int)this->type;
		SC_ADD_PARAMETER(type);
	}
	
	void RigidBody::DeSerialize()
	{
		int type;
		SC_GET_PARAMETER(mass);
		SC_GET_PARAMETER(type);
		this->type = (RigidBodyType)type;
	}
	#ifdef SC_EDITOR_IMPL
	void RigidBody::OnIGUI(Editor::EditorDrawData& dcmd) {
		dcmd.DrawFloat(mass, "Mass");
		dcmd.DrawEnum ((int)type, "Static\0Kinematic\0Dynamic\0\0"s, 3, "Type");
	}

	void RigidBody::PostIGUI(Editor::EditorDrawData& dcmd) {
		mass = *((float*)dcmd.data[0].data);
		int type = *((int*)dcmd.data[1].data);
		this->type = (RigidBodyType)(type);
	}
	#endif

	GET_CID_IMPL(RigidBody);
}
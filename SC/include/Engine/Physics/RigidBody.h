#pragma once

#include "Engine/Core/Core.h"
#include "Engine/ECS/Component.h"
#include "Engine/ECS/Script.h"

struct b2Body;

namespace SC {
	enum class RigidBodyType
	{
		Static,
		Kinematic,
		Dynamic,
	};

	class SC_API RigidBody: public Script
	{
	public:
		RigidBody();
		~RigidBody();

		RigidBodyType type;
		float mass;
	private:
		b2Body* body;

		void FixedUpdate();
		void OnDestroy();
		void Awake();
		friend class Component<RigidBody>;
	};
}

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Event.h"
#include "Engine/ECS/Component.h"
#include "Engine/ECS/Script.h"

struct b2Body;

namespace SC {
	class Application;

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

		RigidBodyType type = RigidBodyType::Static;
		float mass = 1.0f;

		void ApplyParameters();
	private:
		b2Body* body;
		int physicsID;
		EventHandler<> OnApplyParameters;

		void FixedUpdate();
		void OnDestroy();
		void Awake();

		friend class Component<RigidBody>;
		friend class Physics;
		friend class BoxCollider;
		friend class CircleCollider;
	};
}

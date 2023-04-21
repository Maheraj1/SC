#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Event.h"
#include "Engine/ECS/NativeScript.h"

class b2Body;

namespace SC {
	class Application;

	enum class RigidBodyType
	{
		Static,
		Kinematic,
		Dynamic,
	};

	class SC_API RigidBody: public NativeScript
	{
	public:
		RigidBody();
		~RigidBody();

		RigidBodyType type = RigidBodyType::Static;
		float mass = 1.0f;

		void ApplyParameters();

		virtual uint64_t GetCID() override;
	private:
		b2Body* body;
		int physicsID;
		EventHandler<> OnApplyParameters;

		void FixedUpdate();
		void OnDestroy();
		void Awake();
		void Serialize() const override;
		void DeSerialize() override;

		friend class Physics;
		friend class BoxCollider;
		friend class CircleCollider;
	};
}

#pragma once

#include "Engine/Core/Math.h"
#include "Engine/ECS/Script.h"
#include "Collider.h"

struct b2Fixture;
struct b2CircleShape;

namespace SC {
	struct RigidBody;

	class SC_API CircleCollider: public Script, public Collider
	{
		public:
			float size = 1.0f;

			float Friction = .5f;
			float Density = 1.0f;
			float Restitution = 0.0f;
			float RestitutionThreshold = 0.5f;
			bool IsTrigger = false;

			void ApplyParameters();
		private:
			void Start();
			void Serialize() const override;
			void DeSerialize() override;

			RigidBody* rb;
			b2Fixture* fixture;
			b2CircleShape* shape;
		friend class Component<CircleCollider>;
	};
}

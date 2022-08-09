#pragma once

#include "Engine/Core/Math.h"
#include "Engine/ECS/Script.h"
#include "Engine/Scene/Collider.h"

struct b2Fixture;
struct b2PolygonShape;

namespace SC {
	struct RigidBody;

	class SC_API BoxCollider: public Script, public Collider
	{
		public:
			Vector2f offset = Vector2f(0.0f, 0.0f);
			Vector2f size = Vector2f(1.0f, 1.0f);

			float Friction = .5f;
			float Density = 1.0f;
			float Restitution = 0.0f;
			float RestitutionThreshold = 0.5f;
			bool IsTrigger = false;

			void ApplyParameters();
		private:
			void Start();

			RigidBody* rb;
			b2Fixture* fixture;
			b2PolygonShape* shape;
		friend class Component<BoxCollider>;
	};
}

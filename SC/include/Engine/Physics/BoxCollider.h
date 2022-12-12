#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/Script.h"
#include "Collider.h"

class b2Fixture;
class b2PolygonShape;

namespace SC {
	class RigidBody;

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
			void Serialize() const override;
			void DeSerialize() override;

			RigidBody* rb;
			b2Fixture* fixture;
			b2PolygonShape* shape;
		friend class Component<BoxCollider>;
	};
}

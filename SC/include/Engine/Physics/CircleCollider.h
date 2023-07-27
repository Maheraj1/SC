#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/NativeScript.h"
#include "Collider.h"

class b2Fixture;
class b2CircleShape;

namespace SC {
	class RigidBody;

	class SC_API CircleCollider: public NativeScript, public Collider
	{
		public:
			float size = 1.0f;

			float Friction = .5f;
			float Density = 1.0f;
			float Restitution = 0.0f;
			float RestitutionThreshold = 0.5f;
			bool IsTrigger = false;

			void ApplyParameters();

			virtual uint64_t GetCID() override;

			#ifdef SC_EDITOR_IMPL
			virtual void OnIGUI(Editor::EditorDrawData& dcmd) override { }
			virtual void PostIGUI(Editor::EditorDrawData& dcmd) override { }
			#endif
		private:
			void Start() override;
			void Serialize() const override;
			void DeSerialize() override;

			RigidBody* rb;
			b2Fixture* fixture;
			b2CircleShape* shape;
	};
}

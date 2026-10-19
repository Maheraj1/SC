#pragma once

#include "box2d/box2d.h"

#include "Engine/Core/Core.h"
#include "Engine/Core/Event.h"
#include "Engine/ECS/NativeScript.h"

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
		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		#endif
	private:
		b2BodyId body;
		int physicsID;
		EventHandler<> OnApplyParameters;

		void FixedUpdate() override;
		void OnDestroy() override;
		void Awake() override;
		void Serialize() const override;
		void DeSerialize() override;

		friend class Physics;
		friend class BoxCollider;
		friend class CircleCollider;
	};
}

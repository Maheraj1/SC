#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/NativeScript.h"
#include "Engine/ECS/Renderer.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include "Engine/Serialization/ResourceReference.h"
#include "glm/detail/qualifier.hpp"

namespace SC
{
	class SpriteRenderer: public Renderer
	{
	private:
		void Serialize() const override;
		void DeSerialize() override;
		void Start() override;
		void Awake() override;
		void PostRender() override;
		void OnRender() { }
		void OnApplicationStart();

		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override { }
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override { }
		#endif

	public:
		SpriteRenderer();
		~SpriteRenderer() = default;

		virtual uint64_t GetCID() override;

	public:
		Material material;

	friend class Internal::Renderer;
	};
}
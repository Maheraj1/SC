#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/NativeScript.h"
#include "Engine/ECS/Renderer.h"
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
		void Start();
		void Awake();
		void PostRender();
		void OnRender() { }
		void OnApplicationStart();

	public:
		SpriteRenderer();
		~SpriteRenderer() = default;

		virtual uint64_t GetCID() override;

	public:
		ResourceReference<Shader> shader;
		ResourceReference<Texture> texture;
		Color color = {255, 255, 255};

	friend class Internal::Renderer;
	};
}
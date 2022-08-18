#pragma once

#include "Engine/Core/Math.h"
#include "Engine/ECS/Script.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include "glm/detail/qualifier.hpp"

namespace SC
{
	class SpriteRenderer: public Script
	{
	private:
		void Serialize() const override;
		void DeSerialize() override;
	public:
		SpriteRenderer()
		:shader(nullptr), texture(nullptr)
		{ }
		~SpriteRenderer() { }

		void Start();
		void PostRender();
		void OnRender() { }

	public:
		Shader* shader;
		Texture* texture;
		Color color = {255, 255, 255};
	};
}
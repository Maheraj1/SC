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
		
	public:
		SpriteRenderer()  { }
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
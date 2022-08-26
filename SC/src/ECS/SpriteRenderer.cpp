#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Core/Math.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/Resources.h"

#include <string>

namespace SC
{
	void SpriteRenderer::PostRender()
	{
		color.r = 255;
		color.b = 255;
		color.g = 255;
	}

	void SpriteRenderer::Start() { 
		if (shader == nullptr) shader = Resources::GetShaderPtr("Sprite");
		if (texture == nullptr) texture = Resources::GetTexturePtr("Square");
	}

	void SpriteRenderer::Serialize() const
	{
		Color16 color = (Color16)this->color;
		SC_ADD_PARAMETER(color);
	}

	void SpriteRenderer::DeSerialize()
	{
		Color16 color;
		SC_GET_PARAMETER(color);
		this->color = (Color)color;
	}
}

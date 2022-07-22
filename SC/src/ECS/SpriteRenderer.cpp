#include "Engine/ECS/SpriteRenderer.h"
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

	void SpriteRenderer::Start()
	{
		shader = Resources::GetShaderPtr("Sprite");
		if (texture == nullptr)
		{
			texture = Resources::AddTexturePtr("Square", "Sq.png");
			// texture->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGB);
			texture->Generate();
		}
	}
}

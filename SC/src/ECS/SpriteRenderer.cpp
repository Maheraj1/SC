#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Math/Math.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Serialization/SerializedData.h"

#include <string>

namespace SC
{
	SpriteRenderer::SpriteRenderer():Renderer(material) { }

	void SpriteRenderer::PostRender()
	{
		Renderer::mat = material;
	}

	void SpriteRenderer::Start() { }

	void SpriteRenderer::Awake() { }

	void SpriteRenderer::Serialize() const
	{
		SC_ADD_PARAMETER(material);
	}

	void SpriteRenderer::DeSerialize()
	{
		SC_GET_PARAMETER(material);
	}

	GET_CID_IMPL(SpriteRenderer);

	void SpriteRenderer::OnApplicationStart() { 

	}
}

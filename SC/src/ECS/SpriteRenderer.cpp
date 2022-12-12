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
	SpriteRenderer::SpriteRenderer()
	:shader(nullptr), texture(nullptr)
	{

	}

	void SpriteRenderer::PostRender()
	{
		Renderer::texture = texture.obj;
		Renderer::shader  = shader.obj;
		Renderer::color   = color;
	}

	void SpriteRenderer::Start() {
		Renderer::texture = texture.obj;
		Renderer::shader  = shader.obj ;
		Renderer::color   = color;
		texture = Resources::GetResource<Texture> ("Square");
		shader  = Resources::GetResource<Shader>  ("Sprite");
	}

	void SpriteRenderer::Awake() {
		texture = Resources::GetResource<Texture> ("Square");
		shader  = Resources::GetResource<Shader>  ("Sprite");
	}

	void SpriteRenderer::Serialize() const
	{
		SC_ADD_PARAM((Color16)color, "color");

		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

	void SpriteRenderer::DeSerialize()
	{
		Color16 color = {255, 255, 255};
		SC_GET_PARAM(color, "color");
		this->color = (Color)color;

		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

	void SpriteRenderer::OnApplicationStart() { 

	}
}

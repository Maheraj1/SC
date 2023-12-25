#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Math/Math.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Serialization/SerializedData.h"

#include <string>

namespace SC
{
	SpriteRenderer::SpriteRenderer()
	:Renderer(nullptr)
	 { }

	void SpriteRenderer::PreRender()
	{
		Renderer::mat = material.obj;
		Internal::ImQuad quad;
		quad.color = color * material->color;
		quad.texture = material->texture.obj;
		quad.transform = entity->transform;

		Internal::Renderer::RenderQuad(quad, material->shader.obj);
	}

	void SpriteRenderer::Start() { 

	}

	void SpriteRenderer::Awake() { }

	void SpriteRenderer::Serialize() const
	{
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_ADD_PARAM(color, "Color");
	}

	void SpriteRenderer::DeSerialize()
	{	
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_GET_PARAM(color, "Color");
	}

	GET_CID_IMPL(SpriteRenderer);

	void SpriteRenderer::OnApplicationStart() { 

	}

	#ifdef SC_EDITOR_IMPL
	
	void SpriteRenderer::OnIGUI(Editor::EditorDrawData& dcmd) { 
		dcmd.DrawColor((color), "Color");
	}
	
	void SpriteRenderer::PostIGUI(Editor::EditorDrawData& dcmd) { 
		color = (*((Color*)(dcmd.data[0].data)));
	}
	#endif

}

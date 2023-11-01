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
	:Renderer(nullptr) { }

	void SpriteRenderer::PostRender()
	{
		Renderer::mat = material.obj;
		Renderer::blendColor = color;
	}

	void SpriteRenderer::Start() { 

	}

	void SpriteRenderer::Awake() { }

	void SpriteRenderer::Serialize() const
	{
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_ADD_PARAM((Color16)color, "Color");
	}

	void SpriteRenderer::DeSerialize()
	{	
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");

		Color16 color = {0, 0, 0};
		SC_GET_PARAM(color, "Color");
		this->color = color;
	}

	GET_CID_IMPL(SpriteRenderer);

	void SpriteRenderer::OnApplicationStart() { 

	}

	#ifdef SC_EDITOR_IMPL
	
	void SpriteRenderer::OnIGUI(Editor::EditorDrawData& dcmd) { 
		dcmd.DrawColor((color/255.0f), "Color");
	}
	
	void SpriteRenderer::PostIGUI(Editor::EditorDrawData& dcmd) { 
		color = (*((ColorF*)(dcmd.data[0].data))) * 255.0f;
	}
	#endif

}

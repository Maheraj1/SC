#include "Engine/ECS/LineRenderer.h"

namespace SC {
	LineRenderer::LineRenderer() 
	:Renderer(nullptr) { }

	LineRenderer::~LineRenderer() {
	
	}

	void LineRenderer::Serialize() const
	{
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_ADD_PARAM(color, "Color");
	}

	void LineRenderer::DeSerialize()
	{	
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_GET_PARAM(color, "Color");
	}
}
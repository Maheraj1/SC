#include "Engine/ECS/LineRenderer.h"
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Serialization/SerializedData.h"

namespace SC {
	LineRenderer::LineRenderer() 
	:Renderer(nullptr) { }

	LineRenderer::~LineRenderer() {
	
	}

	void LineRenderer::PreRender() {
		Renderer::mat = material.obj;
		Internal::ImLine line;
		// line.color
	}

	void LineRenderer::Serialize() const
	{
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_ADD_PARAM(color, "Color");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&points, "Points");
	}

	void LineRenderer::DeSerialize()
	{	
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_GET_PARAM(color, "Color");
	}

	#ifdef SC_EDITOR_IMPL

	void LineRenderer::OnIGUI(Editor::EditorDrawData& dcmd) {

	}

	void LineRenderer::PostIGUI(Editor::EditorDrawData& dcmd) {

	}

	#endif

	GET_CID_IMPL(LineRenderer);

	LineRenderer::Point::Point() 
	:position(), color()
	{ }

	LineRenderer::Point::Point(Vector2f position, Color color) 
	:position(position), color(color)
	{ }

	void LineRenderer::Point::Serialize() const {
		SC_ADD_PARAMETER(position);
		SC_ADD_PARAMETER(color);
	}

	void LineRenderer::Point::DeSerialize() {
		SC_GET_PARAMETER(position);
		SC_GET_PARAMETER(color);
	}
}
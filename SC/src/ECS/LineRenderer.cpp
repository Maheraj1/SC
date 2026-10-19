#include "Engine/ECS/LineRenderer.h"
#include "Engine/Core/Base.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Serialization/SerializedData.h"

namespace SC {
	LineRenderer::LineRenderer() 
	:Renderer(nullptr) { }

	LineRenderer::~LineRenderer() {
	
	}

	void LineRenderer::PreRender() {
		Renderer::mat = material.obj;
		Internal::ImLine line {
			.points = points.data,
			.thickness = thickness,
			.IsStripped = true,
		};

		Internal::Renderer::RenderLine(line);
	}

	void LineRenderer::Serialize() const
	{
		SC_ADD_PARAM<float>(this->thickness, "Thickness");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&points, "Points");
	}

	void LineRenderer::DeSerialize()
	{	
		SC_GET_PARAM<float>(this->thickness, "Thickness");
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->material, "Material");
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&points, "Points");
	}

	#ifdef SC_EDITOR_IMPL

	void LineRenderer::OnIGUI(Editor::EditorDrawData& dcmd) {
		dcmd.DrawFloat(thickness, "Thickness");
		dcmd.DrawList(points->data(), "Points", points->size(), Editor::EditorType::Custom, sizeof(Point));
	}

	void LineRenderer::PostIGUI(Editor::EditorDrawData& dcmd) {
		thickness = *((float*)dcmd.data[0].data);
		// points.data = *((std::vector<Point>*)(dcmd.data[1].data));
	}

	#endif

	GET_CID_IMPL(LineRenderer);

	Point::Point() 
	:position(), color()
	{ }

	Point::Point(Vector2f position, Color color) 
	:position(position), color(color)
	{ }

	void Point::Serialize() const {
		SC_ADD_PARAMETER(position);
		SC_ADD_PARAMETER(color);
	}

	void Point::DeSerialize() {
		SC_GET_PARAMETER(position);
		SC_GET_PARAMETER(color);
	}

	void Point::OnIGUI(Editor::EditorDrawData& dcmd) {
		dcmd.DrawVector2(position, "Position");
		dcmd.DrawColor	(color, "Color");
	}

	void Point::PostIGUI(Editor::EditorDrawData& dcmd) {
		position = *((Vector2f*)dcmd.data[0].data);
		color = *((Color*)dcmd.data[1].data);
	}
}
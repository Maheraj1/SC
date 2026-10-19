#pragma once

#include "Engine/ECS/IEditorObject.h"
#include "Engine/Serialization/SerializableVector.h"
#include "Engine/Serialization/SerializedData.h"
#include "Renderer.h"
#include <memory>

namespace SC {
		
	struct Point: public IEditorObject, public Serialization::SerializableObject {
		Vector2f position;
		Color color;

		Point();
		Point(Vector2f position, Color color);
		virtual ~Point() = default;
		void Serialize() const override;
		void DeSerialize() override;

		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		#endif
	};

	class LineRenderer: public Renderer {
	  public:
		LineRenderer();
		~LineRenderer();

		virtual uint64_t GetCID() override;
		
	private:
		void Serialize() const override;
		void DeSerialize() override;
		void Start() override { }
		void Awake() override { }
		void PreRender() override;
		void OnRender() { }
		void OnApplicationStart();

		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		#endif
	
	public:
		float thickness;
		ResourceReference<Material> material;
		SerializableVector<std::unique_ptr<Point>> points;

		friend class Internal::Renderer;
	};
}
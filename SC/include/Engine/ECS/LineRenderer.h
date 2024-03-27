#pragma once

#include "Engine/Serialization/SerializableVector.h"
#include "Engine/Serialization/SerializedData.h"
#include "Renderer.h"

namespace SC {
	class LineRenderer: public Renderer {
	  public:
		LineRenderer();
		~LineRenderer();

		virtual uint64_t GetCID() override;

		struct Point;
		
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
		ResourceReference<Material> material;
		SerializableVector<Point> points;
		Color color;

		struct Point: public SerializableObj {

			Vector2f position;
			Color color;

			Point();
			Point(Vector2f position, Color color);
			virtual ~Point() = default;
			void Serialize() const override;
			void DeSerialize() override;
		};

		friend class Internal::Renderer;
	};
}
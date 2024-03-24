#pragma once

#include "Renderer.h"

namespace SC {
	class LineRenderer: public Renderer {
	  public:
		LineRenderer();
		~LineRenderer();

		virtual uint64_t GetCID() override;
		
	private:
		void Serialize() const override;
		void DeSerialize() override;
		void Start() override;
		void Awake() override;
		void PreRender() override;
		void OnRender() { }
		void OnApplicationStart();

		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		#endif
	
	public:
		ResourceReference<Material> material;
		std::vector<Vector2f> points;
		Color color;

		friend class Internal::Renderer;
	};
}
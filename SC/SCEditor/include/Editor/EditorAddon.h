#pragma once

#include "Editor/Console.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Internal/ApplicationAddons.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Scene/Scene.h"

#define EDITOR_I EditorAddon::instance

namespace SC::Editor {
	enum class Tool {
		Translate,
		Rotate,
		Scale
	};

	class EditorAddon: public ApplicationAddon
	{
		public:
			EditorAddon() = default;
			
			virtual void OnKeyDown(OnKeyDownArgs args) override;
			virtual void OnMouseButtonDown(OnMouseButtonDownArgs args) override;
			virtual void PreFrameRender() override;
			virtual void PostFrameRender() override;
			virtual void Update() override;
			virtual void Start() override;
			virtual void ShutDown() override;

		private:

			bool DrawInspectorComponent(IScript* script);
			void DrawSceneHeirarchy();
			void DrawInspector();
			void DrawToolbar();
			void DrawViewPort();
			void DrawMenubar();
			void DrawStats();
			void SaveScene();
			void DrawGizmos();
			
			Texture* PlayTex;
			Texture* PauseTex;
			Texture* StopTex;


			Vector2f CameraPos = {0, 0};
			float zoomLevel = 10.0f;
			float CameraSpeed = 10.0f;
			Scene originalScene;

			FrameBuffer fb;
			Console console;

			Tool current_tool;
		public:
			UUID entSelectedID = 0;
			Entity* entSelected;
			uint32_t ViewPortTex;
			uint32_t GameViewTex;
			bool IsViewPortSelected = false;
			Vector2i ViewPortSize;
			bool ViewPortOpen;
			bool GameViewOpen;
			bool sceneUnSaved = false;
			bool enableGizmos = true;

			static EditorAddon* instance;

	};
}

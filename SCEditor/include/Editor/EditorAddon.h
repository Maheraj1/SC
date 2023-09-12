#pragma once

#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Internal/ApplicationAddons.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Scene/Scene.h"

namespace SC::Editor {
	class EditorAddon: public ApplicationAddons
	{
		public:
			EditorAddon() = default;
			virtual void PreFrameRender() override;
			virtual void PostFrameRender() override;
			virtual void Update() override;
			virtual void Start() override;
			virtual void ShutDown() override;

		private:

			void DrawInspectorComponent(IScript* script);
			void DrawInspector();
			void DrawToolbar();
			void DrawViewPort();
			void DrawMenubar();
			void DrawStats();
			
			Texture* PlayTex;
			Texture* PauseTex;
			Texture* StopTex;

			Vector2f CameraPos = {0, 0};
			float zoomLevel = 10.0f;
			float CameraSpeed = 10.0f;
			Scene originalScene;

			FrameBuffer fb;
	};
}

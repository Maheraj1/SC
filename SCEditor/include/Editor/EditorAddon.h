#pragma once

#include "Engine/Math/Math.h"
#include "Engine/Internal/ApplicationAddons.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Texture.h"

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

			void DrawInspector();
			
			Texture* PlayTex;
			Texture* PauseTex;
			Texture* StopTex;

			Vector2f CameraPos = {0, 0};
			float zoomLevel = 10.0f;
			float CameraSpeed = 10.0f;

			FrameBuffer fb;
	};
}

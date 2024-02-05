#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace SC::Internal
{

	static void RenderScene(const CameraData& data) {
		// calculate mvp for camera [Here v: view; p: projection are calculated because the batch renderer (While processing) calculates m: model]
		float aspectRatio = (float)data.WindowSize.x / (float)data.WindowSize.y;
		Matrix4 proj = glm::ortho(-aspectRatio * data.zoomLevel, aspectRatio * data.zoomLevel, -data.zoomLevel, data.zoomLevel);
		
		Matrix4 view(1.0f);
		view = glm::translate(view, {data.pos.x, data.pos.y, 1.0f});
		view = glm::rotate(view, data.Rotation, {0.0f, 0.0f, 1.0f});
		
		// Set camera data
		Renderer::SetMVP(view*proj);

		// Render current scene
		Renderer::Render();
	}

	uint32_t SceneRenderer::Render(const CameraData& data) {
		// Create a empty FrameBuffer with size of screen
		FrameBuffer fb(data.WindowSize.x, data.WindowSize.y);
		
		if (!data.RenderToScreen) {
			fb.Init();
			fb.Bind();
		}

		Application::GetWindow().Clear(data.clear_color);
		RenderScene(data);

		if (!data.RenderToScreen) {
			fb.UnBind();
			return fb.GetTextureID();
		}

		return 0;
	}

	uint32_t SceneRenderer::Render(const CameraData& data, FrameBuffer& fb) {
		Application::GetWindow().Clear(data.clear_color);
		fb.Bind();
		Vector2 WSize = Application::GetWindow().GetWindowSize();

		Application::GetWindow().SetWindowSize(data.WindowSize);
		Application::GetWindow().Clear(data.clear_color);
		
		RenderScene(data);

		fb.UnBind();
		Application::GetWindow().SetWindowSize(WSize);
		return fb.GetTextureID();
	}
}
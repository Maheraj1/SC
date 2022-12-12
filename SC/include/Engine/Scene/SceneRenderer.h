#pragma once

#include "Engine/Math/Math.h"
#include "Engine/Renderer/FrameBuffer.h"

namespace SC::Internal
{
	struct Entity;
	struct CameraData
	{
		Vector2f pos;
		Vector2i WindowSize = {1280, 720};
		float Rotation;
		float zoomLevel = 1;
		bool RenderToScreen;
	};

	class SceneRenderer
	{
		public:
			/**
			 * @brief A method which can render a scene with custom camera data
			 *
			 * @param data custom data of the camera
			 * @returns uint32_t texture that it has been rendered on
			 */
			static uint32_t Render(const CameraData& data);

			/**
			 * @brief A method which can render Entities (mostly a scene) with custom camera data
			 * (This will ignore SC::Internal::CameraData::RenderToScreen)
			 * 
			 * @param data custom data of the camera
			 * @returns uint32_t texture that it has been rendered on
			 */
			static uint32_t Render(const CameraData& data, FrameBuffer& fb);
	};
}
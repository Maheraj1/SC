#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Debug/Timmer.h"

#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Serialization/SerializedData.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <climits>

namespace SC 
{
	Camera* Camera::instance = nullptr;
	Matrix4 Camera::GetViewProjection()
	{
		return ViewProjection;
	}

	void Camera::ReCalculateViewProjection()
	{
		Matrix4 proj = glm::ortho(-16.0f*size, 16.0f*size, -9.0f*size, 9.0f*size);
		Matrix4 view = entity->transform.GetModel(false);
		ViewProjection = view * proj;
		Internal::Renderer::SetMVP(ViewProjection);
	}

	void Camera::Update()
	{
		if (size <= 0) size = 1;
		ReCalculateViewProjection();
	}

	void Camera::Start()
	{
		if (instance == nullptr) {
			instance = this;
		}
		else {
			entity->RemoveComponent<Camera>();
			return;
		}
		ReCalculateViewProjection();
	}

	void Camera::OnDestroy()
	{
		if (instance == this) instance = nullptr;
	}

	void Camera::Serialize() const
	{
		SC_ADD_PARAMETER(size);
		SC_ADD_PARAMETER(RenderToImage);
	}

	void Camera::OnApplicationStart() {
		fb.Init();
	}

	void Camera::DeSerialize()
	{
		SC_GET_PARAMETER(size);
		SC_GET_PARAMETER(RenderToImage);
	}

	void Camera::Render()
	{
		Internal::CameraData data;
		data.pos = transform->position;
		data.WindowSize = Internal::Renderer::Resolution;
		data.RenderToScreen = true;
		data.zoomLevel = size;
		data.Rotation = transform->rotation;
		fb.Invalidate();

		if ((RenderToImage) || (Application::IsEditor && Application::EditMode))
			Internal::SceneRenderer::Render(data, fb);
		else
			Internal::SceneRenderer::Render(data);
	}

	uint32_t Camera::GetRenderTexture()
	{
		return texID;
	}

	Camera::Camera()
	:fb(1280, 720)
	{ 
		SceneManager::GetCurrentScene().activeCamera = this;
	}
}
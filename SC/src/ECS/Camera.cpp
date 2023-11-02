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
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <climits>

namespace SC 
{
	Camera* Camera::instance = nullptr;
	Matrix4 Camera::GetViewMatrix()
	{
		glm::mat4 mat(1.0f);
		mat = glm::translate(mat, {entity->transform.position.x, entity->transform.position.y, 1.0f});
		mat = glm::rotate(mat, entity->transform.rotation, {0.0f, 0.0f, 1.0f});
		return mat;
	}

	Matrix4 Camera::GetProjectionMatrix(Vector2i window_size)
	{
		Vector2i ws;
		if (window_size.x <= 0 || window_size.y <= 0)
			ws = Internal::Renderer::Resolution;
		else
			ws = window_size;
		
		float aspectRatio = (float)ws.x / (float)ws.y;
		Matrix4 proj = glm::ortho(-aspectRatio * size, aspectRatio * size, -size, size);
		
		return proj;
	}

	void Camera::Update()
	{
		if (size <= 0) size = 1;
	}

	void Camera::Start()
	{
		if (instance == nullptr) {
			instance = this;
		}
		else {
			// entity->RemoveComponent<Camera>();
			return;
		}
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
		data.pos = entity->transform.position;
		data.WindowSize = Internal::Renderer::Resolution;
		data.RenderToScreen = true;
		data.zoomLevel = size;
		data.Rotation = entity->transform.rotation;
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

	GET_CID_IMPL(Camera);

	Camera::Camera()
	:fb(1280, 720)
	{
		SceneManager::GetCurrentScene().activeCamera = this;
	}

	#ifdef SC_EDITOR_IMPL
	void Camera::OnIGUI(Editor::EditorDrawData& dcmd) {
		dcmd.DrawFloat(size, "Projection size");
	}

	void Camera::PostIGUI(Editor::EditorDrawData& dcmd) {
		size = *((float*)dcmd.data[0].data);
	}
	#endif
}
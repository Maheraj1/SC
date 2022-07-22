#include "Engine/Core/Math.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/ext/matrix_transform.hpp"

#include <climits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SC 
{
	Camera* Camera::instance = nullptr;
	Matrix4 Camera::GetViewProjection()
	{
		return ViewProjection;
	}

	void Camera::ReCalculateViewProjection()
	{
		Matrix4 proj = glm::ortho(-16.0f, 16.0f, 9.0f, -9.0f);
		Matrix4 view = entity->transform.GetModel(false);
		ViewProjection = view * proj;
		Internal::Renderer::SetMVP(ViewProjection);
	}

	void Camera::Update()
	{
		ReCalculateViewProjection();
	}

	void Camera::Start()
	{
		if (instance == nullptr) {
			instance = this;
			SceneManager::GetCurrentScene().activeCamera = this;
		}
		else {
			entity->RemoveComponent<Camera>();
			return;
		}
		ReCalculateViewProjection();
	}

	void Camera::OnDestroy()
	{
		instance = nullptr;
	}
}
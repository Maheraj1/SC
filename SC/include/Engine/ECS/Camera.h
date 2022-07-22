#pragma once

#include "Engine/Core/Math.h"
#include "Engine/ECS/IComponent.h"
#include "Script.h"

namespace SC 
{
	class Camera: public Script
	{
	private:
		Matrix4 ViewProjection;
		static Camera* instance;

		void Start();
		void Update();
		void OnDestroy();
	public:
		Camera()  { }
		~Camera() { }

		Matrix4 GetViewProjection();
		void ReCalculateViewProjection();

		friend class Component<Camera>;
	};
}

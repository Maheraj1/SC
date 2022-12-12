#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Scene/Scene.h"
#include "Script.h"

namespace SC 
{
	class Texture;
	class Camera: public Script
	{
	private:
		uint32_t texID;
		Matrix4 ViewProjection;
		static Camera* instance;

		void Start();
		void Update();
		void OnDestroy();
		void Render();
		void OnApplicationStart();

		virtual void Serialize() const override;
		virtual void DeSerialize() override;
	public:
		Camera();
		~Camera() { }

		Matrix4 GetViewProjection();
		void ReCalculateViewProjection();
		uint32_t GetRenderTexture();
		
		float size = 5.0f;
		bool RenderToImage  = true;
		FrameBuffer fb;

		friend class Component<Camera>;
		friend class Window;
	};
}

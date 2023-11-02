#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Scene/Scene.h"
#include "NativeScript.h"

namespace SC 
{
	class Texture;
	class Camera: public NativeScript
	{
	private:
		uint32_t texID;
		static Camera* instance;

		void Start() override;
		void Update() override;
		void OnDestroy() override;
		void Render();
		void OnApplicationStart();

		virtual void Serialize() const override;
		virtual void DeSerialize() override;
	public:
		Camera();
		~Camera() { }

		Matrix4 GetViewMatrix();
		Matrix4 GetProjectionMatrix(Vector2i window_size = Vector2i{0, 0});
		uint32_t GetRenderTexture();

		virtual uint64_t GetCID() override;
		
		#ifdef SC_EDITOR_IMPL
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		#endif
		
		float size = 5.0f;
		bool RenderToImage  = true;
		FrameBuffer fb;
		friend class Window;
	};
}

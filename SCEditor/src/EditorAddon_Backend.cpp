#include "Editor/ContentBrowser.h"
#include "Editor/EditorAddon.h"

#include "ImGuizmo.h"
#include "UI/SCImGui.h"

#include "Engine/Input/KeyCode.h"
#include "Engine/Input/Input.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Time.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneRenderer.h"

#include "Engine/Renderer/Renderer.h"

namespace SC::Editor {

	void EditorAddon::OnKeyDown(OnKeyDownArgs args) {
		switch (args.key) {
			
			case KeyCode::S:
				if (args.modifiers & (uint)ModKey::Control)
					SaveScene();
				break;
			
			case KeyCode::E:
				break;
			case KeyCode::R:
				break;
			case KeyCode::T:
				break;

			default:
				break;
		}
	}

	void EditorAddon::PreFrameRender() {	
		Internal::CameraData data;
		data.RenderToScreen = false;
		data.pos = CameraPos;
		data.Rotation = 0;
		data.WindowSize = ViewPortSize;
		data.zoomLevel = zoomLevel;

		fb.Resize(ViewPortSize);
		
		if (ViewPortOpen) 
			ViewPortTex = Internal::SceneRenderer::Render(data, fb);
	}

	void EditorAddon::Update() {
		Vector2 move = {0, 0};
		
		if (Input::GetKey(KeyCode::W)) move.y -= 1;
		if (Input::GetKey(KeyCode::S)) move.y += 1;
		
		if (Input::GetKey(KeyCode::A)) move.x += 1;
		if (Input::GetKey(KeyCode::D)) move.x -= 1;

		glm::normalize(move);

		CameraPos += move * CameraSpeed * (float)Time::deltaTime;

	}

	void EditorAddon::Start()
	{
		using namespace Internal;

		ViewPortSize = {1280, 720};

		UI::ImGui::Init(Application::GetWindow().GetNativeWindow());
		Application::Get()->_EditMode = true;
		Application::Get()->_IsEditor = true;

		PlayTex  = new Texture("EditorAssets/PlayButton.png");
		PauseTex = new Texture("EditorAssets/PauseButton.png");
		StopTex  = new Texture("EditorAssets/StopButton.png");

		PlayTex->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGBA);
		PauseTex->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGBA);
		StopTex->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGBA);
		
		PlayTex->  Generate();
		PauseTex-> Generate();
		StopTex->  Generate();

		fb.Init();

		instance = this;

		ContentBrowser_Init();
		console.Init();
	}

	void EditorAddon::ShutDown()
	{
		using namespace Internal;
		
		UI::ImGui::ShutDown();
	}

	void EditorAddon::SaveScene() {
		SceneManager::GetCurrentScene().Save();
		EDITOR_I->sceneUnSaved = false;
	}
}
#include "EditorAddon.h"
#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Time.h"
#include "Engine/Input/Input.h"
#include "Engine/Internal/UI/ImGui.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Scene/SceneSerializer.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include <algorithm>

namespace SC::Editor {
	static bool IsViewPortSelected = false;
	static Vector2i ViewPortSize = {1280, 720};
	
	static uint32_t ViewPortTex;
	static uint32_t GameViewTex;
	
	static bool ViewPortOpen;
	static bool GameViewOpen;

	static bool ViewPortSelected = true;

	void EditorAddon::PreFrameRender() 
	{	
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

	void EditorAddon::PostFrameRender() 
	{
		GameViewTex = SceneManager::GetCurrentScene().GetCurrentCamera()->fb.GetTextureID();

		// ImGui Frame
		using namespace ::SC::Internal::UI;
		ImGui::BeginFrame();

		// MenuBar
		ImGui::BeginMenuBar();

			
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Quit", "Cmd + Q");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

		// Toolbar
		ImGui::PushStyleVar(ImGui::ImGuiStyleVar::WindowPadding, {0, 2});
		ImGui::PushStyleVar(ImGui::ImGuiStyleVar::ItemInnerSpacing, {0, 0});

		ImGui::PushStyleColor(ImGui::ImGuiCol::Button, {0, 0, 0, 0});

		auto col =  ImGui::GetStyleColors();
		
		auto col_active = col[(int)ImGui::ImGuiCol::ButtonActive];
		col_active.w = 0.5f;
		ImGui::PushStyleColor(ImGui::ImGuiCol::ButtonActive, col_active);

		auto col_hovered = col[(int)ImGui::ImGuiCol::ButtonHovered];
		col_hovered.w = 0.5f;
		ImGui::PushStyleColor(ImGui::ImGuiCol::ButtonHovered, col_hovered);

		ImGui::Begin("Toolbar", nullptr, (int)ImGui::ImGuiWindowFlags::NoDecoration | 
												(int)ImGui::ImGuiWindowFlags::NoScrollWithMouse);

		static const float ButtonSize = 32.0f;
		const float offset = (ImGui::GetWindowContentRegionMax().x * 0.5f) - ButtonSize;

		// TODO: Make Scene Pause Work

		ImGui::SameLine(offset);

		void* tex = reinterpret_cast<void*>(PlayTex-> GetTextureID());
		Application* app = Application::Get();

		if (!app->EditMode) {
			tex = reinterpret_cast<void*>(StopTex-> GetTextureID());
		}
		
		if (ImGui::ImageButton(tex, {32.0f, 32.0f})) {
			app->SetRunState(!app->EditMode);
		}
		
		ImGui::SameLine(offset+ButtonSize+4);
		ImGui::ImageButton(reinterpret_cast<void*>(PauseTex->GetTextureID()), {32.0f, 32.0f});

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();

		ImGui::PushStyleVar(ImGui::ImGuiStyleVar::WindowPadding, {0.0f, 0.0f});

		// ViewPort
		ImGui::Begin("ViewPort");
		
		ViewPortOpen = true;
		ViewPortSize = ImGui::GetContentRegionAvail();

		ImGui::Image(reinterpret_cast<void*>(ViewPortTex), ImGui::GetContentRegionAvail());
		
		ImGui::End();

		// GameViewPort
		ImGui::Begin("Game", nullptr, (int)ImGui::ImGuiWindowFlags::HorizontalScrollbar);
		ImGui::SetCursorPos((ImGui::GetContentRegionAvail() - (Vector2f)Internal::Renderer::Resolution) * 0.5f);

		ImGui::Image(reinterpret_cast<void*>(GameViewTex), Internal::Renderer::Resolution);
		
		IsViewPortSelected = ImGui::IsCurrentWindowFocusedAndHovered();
		GameViewOpen = ImGui::IsCurrentWindowVisible();
		
		ImGui::End();
		
		ImGui::PopStyleVar();

		// Stats
		ImGui::Begin("Stats");
		
		ImGui::Text("FPS: %f", Time::fps);
		ImGui::Text("DeltaTime: %f", Time::deltaTime);

		ImGui::End();

		ImGui::EndFrame();

		Input::SetState(IsViewPortSelected);
	}

	void EditorAddon::Update() {
		Vector2 move = {0, 0};
		
		if (Input::GetKey_UnBlocked(KeyCode::W)) move.y -= CameraSpeed * Time::deltaTime;
		if (Input::GetKey_UnBlocked(KeyCode::S)) move.y += CameraSpeed * Time::deltaTime;
		
		if (Input::GetKey_UnBlocked(KeyCode::A)) move.x += CameraSpeed * Time::deltaTime;
		if (Input::GetKey_UnBlocked(KeyCode::D)) move.x -= CameraSpeed * Time::deltaTime;

		// glm::normalize(move);

		CameraPos += move;

	}

	void EditorAddon::Start() 
	{
		using namespace ::SC::Internal::UI;
		ImGui::Init(Application::GetWindow().GetNativeWindow());
		Application::Get()->_EditMode = true;
		Application::Get()->_IsEditor = true;

		PlayTex  = Resources::AddResource<Texture>("PlayButton", "EditorAssets/PlayButton.png");
		PauseTex = Resources::AddResource<Texture>("PauseButton", "EditorAssets/PauseButton.png");
		StopTex  = Resources::AddResource<Texture>("StopButton", "EditorAssets/StopButton.png");
		
		PlayTex->  Generate();
		PauseTex-> Generate();
		StopTex->  Generate();

		fb.Init();
	}

	void EditorAddon::ShutDown()
	{
		using namespace ::SC::Internal::UI;
		ImGui::ShutDown();
	}

}

#include "Editor/EditorAddon.h"
#include "Engine/Core/Application.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Input/KeyCode.h"
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
#include "Engine/Core/Platform.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"

#include "imgui.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <xlocale/_stdio.h>

namespace SC::Editor {
	static bool IsViewPortSelected = false;
	static Vector2i ViewPortSize = {1280, 720};
	
	static uint32_t ViewPortTex;
	static uint32_t GameViewTex;
	
	static bool ViewPortOpen;
	static bool GameViewOpen;
	static bool sceneUnSaved = false;

	static UUID entSelectedID = 0;
	static Entity* entSelected;

	static const char* runtime_scene_path = "Data/rnscne.scsr";

	void SaveScene() {
		SceneManager::GetCurrentScene().Save();
		sceneUnSaved = false;
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

		if (Input::GetKey(KeyCode::S) && Input::GetKey(KeyCode::LEFT_CONTROL) && sceneUnSaved) SaveScene();
	}

	void DrawEntity(Entity* ent) {

		bool open = ImGui::TreeNode(reinterpret_cast<void*>(ent->GetUUID()), "%s", ent->name.c_str());

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				SceneManager::GetCurrentScene().DestroyEntity(ent);

			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked()) {
			entSelected = ent;
			entSelectedID = ent->GetUUID();
		}

		if (open) {
			ImGui::TreePop();
		}
	}

	void DrawSceneHeirarchy() {
		ImGui::Begin("Heirarchy");

		Scene& scene = SceneManager::GetCurrentScene();

		if (ImGui::CollapsingHeader(((sceneUnSaved ? "*": "")+ "Scene"s).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen)) {
			for (auto&& ent : scene.GetEntities()) {
				DrawEntity(ent);
			}
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			entSelected = nullptr;
			entSelectedID = 0llu;
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				scene.AddEntity("New Entity");

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void DrawIGUIComponents(EditorDrawCommand& cmd) {
		bool change = false;
		ImGui::Text("%s", cmd.name.c_str());
		ImGui::SameLine();

		std::string id = ("##" + cmd.name);

		switch (cmd.type) {
			case EditorType::None:
				break;
			case EditorType::Int:
				change = ImGui::InputInt(id.c_str(), (int*)cmd.data, ImGuiInputTextFlags_AutoSelectAll);
				break;
			case EditorType::Float:
				change = ImGui::InputFloat(id.c_str(), (float*)cmd.data, .1f, .5f, "%.5f", ImGuiInputTextFlags_AutoSelectAll);
				break;
			case EditorType::Vector2:
				change = ImGui::InputFloat2(id.c_str(), (float*)cmd.data, "%.5f", ImGuiInputTextFlags_AutoSelectAll);
				break;
			case EditorType::Enum:
				change = ImGui::Combo(id.c_str(), (int*)cmd.data, cmd.as_data.c_str());
				break;
		}

		if (!sceneUnSaved)
			sceneUnSaved = change;
	}

	void EditorAddon::DrawInspectorComponent(IScript* script) {
		if (!ImGui::CollapsingHeader((Internal::ComponentData::components[script->GetCID()].qualifiedName + " Component"s).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) return;
		EditorDrawData dcmd;
		script->OnIGUI(dcmd);
		
		for (int i = 0; i < dcmd.data.size(); i++) {
			DrawIGUIComponents(dcmd.data[i]);
		}

		script->PostIGUI(dcmd);
	}

	void EditorAddon::DrawInspector() {
		ImGui::Begin("Inspector");

		if (entSelectedID == 0llu || !entSelected) {
			ImGui::End();
			return;
		}

		ImGui::Text("Name");
		ImGui::SameLine();

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy(buffer, entSelected->name.c_str(), sizeof(buffer));
		
		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
			entSelected->name = std::string(buffer);
		}

		// Transform

		ImGui::Text("Components");

		for (int i = 0; i < entSelected->scripts.size(); i++) {
			DrawInspectorComponent(entSelected->scripts[i]);
		}

		ImGui::End();
	}

	void EditorAddon::DrawToolbar() {
		using namespace ::SC::Internal;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 2});
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0, 0});

		ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});

		auto col =  ImGui::GetStyle().Colors;
		
		auto col_active = col[(int)ImGuiCol_ButtonActive];
		col_active.w = 0.5f;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);

		auto col_hovered = col[(int)ImGuiCol_ButtonHovered];
		col_hovered.w = 0.5f;
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hovered);

		ImGui::Begin("Toolbar", nullptr, (int)ImGuiWindowFlags_NoDecoration | 
												(int)ImGuiWindowFlags_NoScrollWithMouse);

		static const float ButtonSize = 32.0f;
		const float offset = (ImGui::GetWindowContentRegionMax().x * 0.5f) - ButtonSize;

		// TODO: Make Scene Pause Work

		ImGui::SameLine(offset);

		void* tex = reinterpret_cast<void*>(StopTex-> GetTextureID());
		Application* app = Application::Get();

		if (app->EditMode) {
			tex = reinterpret_cast<void*>(PlayTex-> GetTextureID());
		}
		
		if (UI::ImGui::ImageButton(tex, {32.0f, 32.0f})) {
			if (app->EditMode) {
				Scene scene = SceneManager::GetCurrentScene();
				scene.SaveOnOtherPath(runtime_scene_path);
				app->OnAppPlay();
			} else {
				app->OnAppStop();
				SceneManager::GetCurrentScene().LoadFromOtherPath(runtime_scene_path);
				entSelected = nullptr;
				entSelectedID = 0;
			}
		}
		
		ImGui::SameLine(offset+ButtonSize+4);
		UI::ImGui::ImageButton(reinterpret_cast<void*>(PauseTex->GetTextureID()), {32.0f, 32.0f});

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorAddon::DrawViewPort() {
		using namespace ::SC::Internal;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

		// ViewPort
		ImGui::Begin("ViewPort");
		
		ViewPortOpen = true;
		ViewPortSize = UI::ImGui::GetContentRegionAvail();

		UI::ImGui::Image(reinterpret_cast<void*>(ViewPortTex), UI::ImGui::GetContentRegionAvail());
		
		ImGui::End();

		// GameViewPort
		ImGui::Begin("Game", nullptr, (int)ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetCursorPos(UI::ImGui::ToImVec2((UI::ImGui::GetContentRegionAvail() - (Vector2f)Internal::Renderer::Resolution) * 0.5f));

		UI::ImGui::Image(reinterpret_cast<void*>(GameViewTex), Internal::Renderer::Resolution);
		
		IsViewPortSelected = UI::ImGui::IsCurrentWindowFocusedAndHovered();
		GameViewOpen = UI::ImGui::IsCurrentWindowVisible();
		
		ImGui::End();
		
		ImGui::PopStyleVar();
	}

	void EditorAddon::DrawMenubar() {
		ImGui::BeginMenuBar();
			
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save", "ctrl + S")) 
				SaveScene();

			ImGui::Separator();
			
			ImGui::MenuItem("Quit", "Cmd + Q");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	void EditorAddon::DrawStats() {
		ImGui::Begin("Stats");
		
		ImGui::Text("FPS: %0.1f", Time::fps);
		ImGui::Text("DeltaTime: %0.3f ms", Time::deltaTime*1000.0f);

		ImGui::End();
	}

	void EditorAddon::PostFrameRender() {
		GameViewTex = SceneManager::GetCurrentScene().GetCurrentCamera()->fb.GetTextureID();

		using namespace ::SC::Internal;

		// ImGui Frame
		UI::ImGui::BeginFrame();

		DrawMenubar();
		DrawToolbar();
		DrawSceneHeirarchy();
		DrawViewPort();
		DrawInspector();
		DrawStats();

		ImGui::ShowDemoWindow();

		UI::ImGui::EndFrame();
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
		UI::ImGui::Init(Application::GetWindow().GetNativeWindow());
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
		using namespace Internal;
		
		UI::ImGui::ShutDown();
	}

}

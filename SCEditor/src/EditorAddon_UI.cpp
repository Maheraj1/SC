#include "Editor/Console.h"
#include "Editor/ContentBrowser.h"
#include "Editor/EditorAddon.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "UI/SCImGui.h"

#include "Engine/Core/Application.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/IScript.h"
#include "Engine/ECS/Renderer.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Input/KeyCode.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Time.h"
#include "Engine/Input/Input.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Scene/SceneRenderer.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Core/Platform.h"


#include "imgui.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <string>
#include <xlocale/_stdio.h>

namespace SC::Editor {

	EditorAddon* EditorAddon::instance = nullptr;

	static const char* runtime_scene_path = "Data/rnscne.scsr";

	void DrawEntity(Entity* ent) {
		
		auto ent_str_id = std::to_string(ent->GetUUID());

		bool open = ImGui::TreeNode(ent_str_id.c_str(), "%s", ent->name.c_str());

		if (ImGui::BeginPopupContextItem(ent_str_id.c_str(), ImGuiMouseButton_Right)) {
			if (ImGui::MenuItem("Delete Entity"))
				SceneManager::GetCurrentScene().DestroyEntity(ent);

			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {

		}

		if (ImGui::IsItemClicked()) {
			EditorAddon::instance->entSelected = ent;
			EditorAddon::instance->entSelectedID = ent->GetUUID();
		}

		if (open) {
			ImGui::TreePop();
		}
	}

	void EditorAddon::DrawSceneHeirarchy() {
		ImGui::Begin("Heirarchy");

		Scene& scene = SceneManager::GetCurrentScene();

		std::string heirarchy_id = ((EDITOR_I->sceneUnSaved ? "*": "")+ "Scene"s);

		if (ImGui::CollapsingHeader(heirarchy_id.c_str(),
		 ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen)) {
			for (auto&& ent : scene.GetEntities()) {
				DrawEntity(ent);
			}
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
			entSelected = nullptr;
			entSelectedID = 0llu;
		}

		if (ImGui::BeginPopupContextWindow(heirarchy_id.c_str(), ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				scene.AddEntity("New Entity");

			ImGui::EndPopup();
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
		col_active.w = 0.25f;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);

		auto col_hovered = col[(int)ImGuiCol_ButtonHovered];
		col_hovered.w = 0.25f;
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
		
		if (UI::ImGui::ImageButton("Stop-PlayBtn", tex, {32.0f, 32.0f})) {
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
		UI::ImGui::ImageButton("PauseBtn", reinterpret_cast<void*>(PauseTex->GetTextureID()), {32.0f, 32.0f});

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

		ImGui::Text("Entity Selected Id: %llu", (uint64_t)entSelectedID);

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
		DrawContentBrowser();
		console.DrawConsole();

		ImGui::ShowDemoWindow();

		UI::ImGui::EndFrame();
	}

}

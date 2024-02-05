#include "Editor/EditorAddon.h"
#include "UI/SCImGui.h"

#include "Engine/Core/Application.h"
#include "Engine/Scene/SceneManager.h"

#include "imgui.h"

namespace SC::Editor {
	
	static const char* runtime_scene_path = "Data/rnscne.scsr";
	
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
}
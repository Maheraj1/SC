#include "Editor/ContentBrowser.h"

#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/FileSystem.h"
#include "UI/SCImGui.h"

#include "imgui.h"

#include <string>
#include <filesystem>

namespace SC::Editor {
	
	static const std::vector<std::string> hiddenExt = {".scrd"s, ".scsrd"s};
	static const std::string assetsPath = "Assets";
	static const std::string ContentBrowserPath = "EditorAssets/ContentBrowser/";
	static const uint renameFileBufferSize = 256;

	static std::string CurrentPath = assetsPath;
	static std::string SelectedPath = assetsPath;
	static bool IsRenamingCurrentPath = false;
	static char* renameFileBuffer = nullptr;
	
	static Texture* FolderTex;
	static Texture* FileTex;

	// In Engine `hiddenExt`'s context
	bool IsHiddenFile(std::filesystem::path path) {
		if (!path.has_extension()) return false;

		for (int i = 0; i < hiddenExt.size(); i++) {
			if (hiddenExt[i] == path.extension().generic_string()) 
				return true;
		}

		return false;
	}

	void ClearRenameFileBuffer() {
		if (renameFileBuffer)
			free(renameFileBuffer);

		renameFileBuffer = (char*)malloc(renameFileBufferSize);
	}

	void ContentBrowser_Init() {
		auto FolderIconPath = (ContentBrowserPath + "Folder.png");
		auto FileIconPath   = (ContentBrowserPath + "File.png"  );
		FolderTex = new Texture(FolderIconPath.c_str());
		FileTex   = new Texture(FileIconPath.c_str());

		FolderTex->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGBA);
		FileTex->SetAttribute(TextureParameters::Format, TextureProperties::FormatRGBA);
		
		FolderTex->Generate();
		FileTex  ->Generate();

		ClearRenameFileBuffer();
	}


	void DrawContentBrowser() {
		static float padding = 16.0f;
		static float thumbnail_size = 128.0f;
		
		ImGui::Begin("Content Browser");

		ImGui::Text("Icon Size");
		ImGui::SameLine();
		ImGui::SliderFloat("##IconSize", &thumbnail_size, 64, 128);

		if (CurrentPath != assetsPath) {
			if (ImGui::ArrowButton("Back Arrow", ImGuiDir_Left)) {
				CurrentPath = std::filesystem::path(CurrentPath).parent_path();
			}
		}
		
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight)) {

			if (ImGui::Button("New Folder")) {
				
				std::filesystem::create_directory(CurrentPath + "/New Folder");
				SelectedPath = CurrentPath + "/New Folder";
				IsRenamingCurrentPath = true;

				ClearRenameFileBuffer();
				strcpy(renameFileBuffer, SelectedPath.data());

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		
		float cellSize = thumbnail_size + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto col =  ImGui::GetStyle().Colors;
		
		auto col_active = col[(int)ImGuiCol_ButtonActive];
		col_active.w = 0.25f;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, col_active);

		auto col_hovered = col[(int)ImGuiCol_ButtonHovered];
		col_hovered.w = 0.25f;
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hovered);

		for (auto&& p: std::filesystem::directory_iterator(CurrentPath)) {
			
			auto path = p.path();
			auto relativePath = std::filesystem::relative(path, assetsPath);
			auto filenamePath = relativePath.filename().generic_string();

			if (IsHiddenFile(filenamePath)) continue;
				
			auto icon = p.is_directory() ? FolderTex : FileTex;

			ImGui::ImageButton(filenamePath.c_str(), icon->GetTexture(), {thumbnail_size, thumbnail_size} );

			if (ImGui::IsItemHovered()) {

				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					if (p.is_directory()) {
						CurrentPath = path.relative_path().generic_string();
					}
				}
	
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup((filenamePath + "1").c_str());
				}
			}

			// Popup
			if (ImGui::BeginPopup((filenamePath + "1").c_str())) {

				if (ImGui::Button("Rename")) {
					SelectedPath = filenamePath;
					IsRenamingCurrentPath = true;

					ClearRenameFileBuffer();
					strcpy(renameFileBuffer, SelectedPath.data());
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Delete")) {
					//TODO
					ImGui::CloseCurrentPopup();
				}
				
				ImGui::EndPopup();
			}

			bool save = false;


			if (IsRenamingCurrentPath && SelectedPath == filenamePath) {
				ImGui::SetKeyboardFocusHere();
				save = ImGui::InputText(("##" + filenamePath).c_str(), renameFileBuffer, renameFileBufferSize,
				 ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
			}
			else
				ImGui::TextWrapped("%s", filenamePath.c_str());

			ImGui::NextColumn();

			if (save) {
				IsRenamingCurrentPath = false;
				std::filesystem::rename(CurrentPath + "/" + SelectedPath, CurrentPath + "/" + renameFileBuffer);
			}
		}
		ImGui::PopStyleColor(3);

		ImGui::End();
	}
}
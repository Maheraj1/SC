#include "Editor/EditorAddon.h"
#include "imgui.h"

#pragma region Defines

#define SelectComponent \
	if (cid) {\
	EditorAddon::instance->entSelected->AddComponent(cid);\
	EditorAddon::instance->sceneUnSaved = true;}\
	ImGui::CloseCurrentPopup();

#define SelectMonoComponent \
	if (mono_id) {\
	EditorAddon::instance->entSelected->AddMonoComponent(mono_id);\
	EditorAddon::instance->sceneUnSaved = true;}\
	ImGui::CloseCurrentPopup();

#pragma endregion

namespace SC::Editor {

	bool DrawAddComponentPopup() {

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Add Component..")) {
			
			static uint64_t cid = 0;
			static uint64_t mono_id = 0;
			
			if (ImGui::CollapsingHeader("Native Script", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				// last in the list is MonoScript
				for (int i = 0; i < Internal::ComponentData::components.size()-1; i++) {
					auto& comp = Internal::ComponentData::components[i];
					
					auto flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					if (cid == comp.cid) flags |= ImGuiTreeNodeFlags_Selected;

					ImGui::TreeNodeEx((comp.qualifiedName + " ##" + std::to_string(comp.cid)).c_str(), flags);
					
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
						cid = comp.cid;
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() &&
						ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						SelectComponent
					}
				}
			}

			if (ImGui::CollapsingHeader("Mono Script", ImGuiTreeNodeFlags_SpanAvailWidth)) {
				auto data = Scripting::ScriptEngine::GetData();

				for (int i = 0; i < data->scripts.size(); i++) {
					auto& comp = data->scripts[i];
					
					auto flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

					if (mono_id == comp.id) flags |= ImGuiTreeNodeFlags_Selected;

					ImGui::TreeNodeEx((comp.name + " ##" + std::to_string((uint64_t)comp.id)).c_str(), flags);
					
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
						mono_id = comp.id;
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() &&
						ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						SelectMonoComponent
					}
				}
			}

			uint btnSize = 20;
			
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() - btnSize);

			if (ImGui::Button("Ok", ImVec2(30, btnSize)) && cid) {
				SelectComponent
				SelectMonoComponent
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(50, btnSize))) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return true;
	}

	bool DrawVector2(std::string label, Vector2& vec, bool drawLabel = true) {
		
		// <Label> X: 5.0f Y: 5.0f

		bool change = false;
		if (drawLabel) {
			ImGui::Text("%s", label.c_str());
			ImGui::SameLine();
		}

		ImGui::PushItemWidth(100);
		ImGui::Text("X: ");
		ImGui::SameLine();
		change = ImGui::DragFloat(("##X" + label).c_str(), &vec.x) ? true: change;
		ImGui::SameLine();

		ImGui::Text("Y: ");
		ImGui::SameLine();
		change = ImGui::DragFloat(("##Y" + label).c_str(), &vec.y) ? true: change;
		ImGui::PopItemWidth();

		return change;
	}

	void DrawIGUIComponents(EditorDrawCommand& cmd, bool count_change = true) {
		bool change = false;
		ImGui::Text("%s", cmd.name.c_str());
		ImGui::SameLine();

		std::string id = ("##" + cmd.name);

		switch (cmd.type) {
			case EditorType::None:
				break;
			case EditorType::Int:
				change = ImGui::DragInt(id.c_str(), (int*)cmd.data);
				break;
			case EditorType::Bool:
				change = ImGui::Checkbox(id.c_str(), (bool*)cmd.data);
				break;
			case EditorType::Float:
				change = ImGui::DragFloat(id.c_str(), (float*)cmd.data, .2f);
				break;
			case EditorType::Vector2:
				change = DrawVector2(cmd.name, *(Vector2*)cmd.data, false);
				break;
			case EditorType::Enum:
				change = ImGui::Combo(id.c_str(), (int*)cmd.data, cmd.as_data.c_str());
				break;
			case EditorType::Color:
				change = ImGui::ColorEdit3(id.c_str(), (float*)cmd.data, 
					ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_InputRGB |
					 ImGuiColorEditFlags_Float);
				break;
		}

		if (!EDITOR_I->sceneUnSaved && count_change)
			EDITOR_I->sceneUnSaved = change;
	}

	bool EditorAddon::DrawInspectorComponent(IScript* script) {
		bool delete_current_component = false;

		std::string comp_id = (Internal::ComponentData::components[script->GetCID()].qualifiedName + 
								" Component"s);

		if (ImGui::CollapsingHeader(comp_id.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) 
		{

			EditorDrawData dcmd;
			script->OnIGUI(dcmd);
			
			for (int i = 0; i < dcmd.data.size(); i++) {
				DrawIGUIComponents(dcmd.data[i]);
			}

			script->PostIGUI(dcmd);


		}
		
		if (ImGui::BeginPopupContextItem(comp_id.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::Button("Remove Component?")) {
				delete_current_component = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return delete_current_component;
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

			sceneUnSaved = true;
		}

		// Transform
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen)) {
			bool change = false;
			change = DrawVector2("Position", entSelected->transform.position) ? true: change;
			change = DrawVector2("Scale"   , entSelected->transform.scale   ) ? true: change;

			ImGui::Text("Rotation");
			ImGui::SameLine();
			change = ImGui::DragFloat("##Rotation", &entSelected->transform.rotation, 0.1f) ? true: change;

			sceneUnSaved = change ? true: sceneUnSaved;
		}

		// Component View

		ImGui::Text("Components");
		uint64_t delete_comp_cid = 0;

		for (int i = 0; i < entSelected->scripts.size(); i++) {
			if (DrawInspectorComponent(entSelected->scripts[i]))
				delete_comp_cid = entSelected->scripts[i]->GetCID();
			
			ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
		}

		if (delete_comp_cid) {
			entSelected->RemoveComponent(delete_comp_cid);
			sceneUnSaved = true;
		}

		// Add Component

		ImGui::SetCursorPos({(ImGui::GetWindowContentRegionMax().x - 100) * 0.5f, ImGui::GetCursorPos().y});

		if (ImGui::Button("Add Component", {128.0f, 18.0f})) {
			ImGui::OpenPopup("Add Component..");
		}

		DrawAddComponentPopup();

		ImGui::Spacing();

		// Material View

		Renderer* renderer =  entSelected->GetAvailRenderer();
		
		if (renderer) {
			if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen)) {
				EditorDrawData dcmd;
				renderer->mat->OnIGUI(dcmd);
				
				for (int i = 0; i < dcmd.data.size(); i++) {
					DrawIGUIComponents(dcmd.data[i], false);
				}

				renderer->mat->PostIGUI(dcmd);
			}
		}

		ImGui::End();
	}
}

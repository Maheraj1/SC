#include "Editor/Console.h"
#include "Engine/Debug/Debug.h"
#include "imgui.h"
#include <cstdlib>
#include <string>

namespace SC::Editor {

	void Console::DotnetBuildCallback(std::string output) {
		bufferOutput += output;
	}

	void Console::DebugLogCallback(std::string output, LogLevel loglevel) {
		bufferLog.push_back({output, loglevel});
	}

	void Console::Init() {
		Debug::SetBuildCallbackFunction([&](std::string out) { this->DotnetBuildCallback(out); } );
		Debug::editorLogCallbackFunc = ([&](std::string out, LogLevel loglevel) { this->DebugLogCallback(out, loglevel); } );
	}

	void Console::DrawConsole() {
		ImGui::Begin("Console");
		
		if (ImGui::BeginTabBar("##ConsoleTabBar")) {

			if (ImGui::BeginTabItem("Debug Log")) {

				for (int i = 0; i < bufferLog.size(); i++) {
					
					ImColor color = ImGui::GetStyle().Colors[ImGuiCol_Text];
					switch (bufferLog[i].loglevel) {
						case SC::LogLevel::Info:
							color = ImColor{0, 255, 0};
							break;
						case SC::LogLevel::Warning:
							color = ImColor{255, 255, 0};
							break;
						case SC::LogLevel::Error:
							color = ImColor{255, 0, 0};
							break;
						default:
							break;
					}
					
					ImGui::TextColored(color, "%s", bufferLog[i].log.c_str());
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Build Output")) {
				ImGui::TextUnformatted(bufferOutput.data());
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}
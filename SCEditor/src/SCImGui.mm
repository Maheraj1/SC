
#include "Engine/Math/Math.h"
#include <cstdarg>
#include <cstddef>
#if defined(SC_EDITOR_IMPL) && !(SC_CORE_IMPL)

#include "Engine/Core/Platform.h"
#include "UI/SCImGui.h"

#include "imgui.h"

#ifdef SC_WINDOW_API_GLFW
	#include "imgui_impl_glfw.h"
	#include "GLFW/glfw3.h"
#endif

#if defined(SC_RENDERER_Metal)
	#include "imgui_impl_metal.h"
#elif defined(SC_RENDERER_OpenGL)
	#include "imgui_impl_opengl3.h"
#endif

struct GLFWwindow;

namespace SC::Internal::UI::ImGui {

	static GLFWwindow* window;

	void SetColors() {
		auto& colors = ::ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
	
	void Init(void* WindowHandle)
	{
		window = (GLFWwindow*)WindowHandle;
		
		::IMGUI_CHECKVERSION();
		::ImGui::CreateContext();

		::ImGuiIO& io = ::ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		::ImGui::StyleColorsDark();
		SetColors();
		
		#if defined(SC_RENDERER_OpenGL)
		::ImGui_ImplGlfw_InitForOpenGL(window, true);
		::ImGui_ImplOpenGL3_Init("#version 410 core");
		#elif defined(SC_RENDERER_Metal)
        // Metal backend initialization requires a valid MTLDevice which is
        // not available in this translation unit.
        (void)window;
		#endif
	}

	void ShutDown()
	{
		#if defined(SC_RENDERER_OpenGL)
		::ImGui_ImplOpenGL3_Shutdown();
		#elif defined(SC_RENDERER_Metal)
        // Metal backend not initialized (no MTLDevice available).
		#endif
		::ImGui_ImplGlfw_Shutdown();
		::ImGui::DestroyContext();
	}

	void BeginFrame()
	{
		#if defined(SC_RENDERER_OpenGL)
		::ImGui_ImplOpenGL3_NewFrame();
		#elif defined(SC_RENDERER_Metal)
        // Metal backend not initialized.
		#endif
		::ImGui_ImplGlfw_NewFrame();
		::ImGui::NewFrame();
	}

	void EndFrame()
	{
		::ImGui::Render();
		#if defined(SC_RENDERER_OpenGL)
		::ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
		#elif defined(SC_RENDERER_Metal)
        // Metal backend not initialized.
		#endif
	}

	void Begin(const char* name, bool* p_open, int flags)
	{
		::ImGui::Begin(name, p_open, flags);
	}

	void Text(const char* fmt, ...)
	{
		va_list list;
		va_start(list, fmt);
		::ImGui::TextV(fmt, list);
		va_end(list);
	}

	void End()
	{
		::ImGui::End();
	}

	void Image(void* tex, Vector2 size)
	{
		#ifdef SC_RENDERER_OpenGL
		::ImGui::Image(tex, {size.x, size.y}, {0, 1}, {1, 0});
		#else
		::ImGui::Image(tex, {size.x, size.y});
		#endif
	}

	Vector2 GetWindowSize()
	{
		auto s = ::ImGui::GetWindowSize();
		return {s.x, s.y};
	}

	void ShowMetricsWindow()
	{
		::ImGui::ShowMetricsWindow();
	}

	bool IsCurrentWindowFocusedAndHovered()
	{
		return ::ImGui::IsWindowFocused() && ::ImGui::IsWindowHovered();
	}

	bool IsCurrentWindowVisible() {
		return ::ImGui::IsWindowAppearing();
	}

	void ListBox(const char* label, int* currentItem, const char* const *items, int count)
	{
		::ImGui::ListBox(label, currentItem, items, count);
	}

	void PlotLines(const char* label, const float* values, int valuesCount)
	{
		::ImGui::PlotLines(label, values, valuesCount);
	}

	bool Button(const char *label)
	{
		return ::ImGui::Button(label);
	}

	bool ImageButton(const char* str_id, void* tex, Vector2 size)
	{
		#ifdef SC_RENDERER_OpenGL
		return ::ImGui::ImageButton(str_id, tex, {size.x, size.y}, {0, 1}, {1, 0});
		#else
		return ::ImGui::ImageButton(str_id, tex, {size.x, size.y});
		#endif
	}

	// MenuBar Functions
	bool BeginMenuBar() {
		return ::ImGui::BeginMenuBar();
	}

	void EndMenuBar() {
		::ImGui::EndMenuBar();
	}

	bool BeginMainMenuBar() {
		return ::ImGui::BeginMainMenuBar();
	}

	void EndMainMenuBar() {
		::ImGui::EndMainMenuBar();
	}

	bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
		return ::ImGui::MenuItem(label, shortcut, selected, enabled);
	}
	
	bool BeginMenu(const char *label) {
		return ::ImGui::BeginMenu(label);
	}

	void EndMenu() {
		::ImGui::EndMenu();
	}

	// Advanced Functions

	void SameLine(float xOffset, float spacing) {
		::ImGui::SameLine(xOffset, spacing);
	}

	void SetCursorPosX(float pos) {
		::ImGui::SetCursorPosX(pos);
	}

	void SetCursorPosY(float pos) {
		::ImGui::SetCursorPosY(pos);
	}

	void PushStyleVar(ImGuiStyleVar idx, float value) {
		::ImGui::PushStyleVar((int)idx, value);
	}

	void PushStyleVar(ImGuiStyleVar idx, Vector2f value) {
		::ImGui::PushStyleVar((int)idx, {value.x, value.y});
	}

	void PopStyleVar(int n) {
		::ImGui::PopStyleVar(n);
	}

	void PushStyleColor(ImGuiCol idx, Vector4f value) {
		::ImGui::PushStyleColor((int)idx, {value.x, value.y, value.z, value.w});
	}

	void PopStyleColor(int n) {
		::ImGui::PopStyleColor(n);
	}

	Vector2 GetWindowContentRegionMin() {
		return FromImVec2(::ImGui::GetWindowContentRegionMin());
	}
	Vector2 GetWindowContentRegionMax() {
		return FromImVec2(::ImGui::GetWindowContentRegionMax());
	}

	Vector2 GetContentRegionAvail() {
		return FromImVec2(::ImGui::GetContentRegionAvail());
	}

	void SetCursorPos(Vector2 pos) {
		::ImGui::SetCursorPos(ToImVec2(pos));
	}

	Vector4f* GetStyleColors() {
		return (Vector4f*)::ImGui::GetStyle().Colors; 
	}
}

#endif

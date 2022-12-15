#pragma once

#include "Engine/Math/Math.h"
#include <imgui.h>
#include <functional>

#if defined(SC_EDITOR_IMPL) && !(SC_CORE_IMPL)

// A internal class that would be compiled separated with core module (The Main Engine Code) and be linked to Editor or Debug runtimes
namespace SC::Internal::UI::ImGui {

	//* Taken From imgui.h
	enum class ImGuiStyleVar: int {
		// Enum name --------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
		Alpha,               // float     Alpha
		DisabledAlpha,       // float     DisabledAlpha
		WindowPadding,       // ImVec2    WindowPadding
		WindowRounding,      // float     WindowRounding
		WindowBorderSize,    // float     WindowBorderSize
		WindowMinSize,       // ImVec2    WindowMinSize
		WindowTitleAlign,    // ImVec2    WindowTitleAlign
		ChildRounding,       // float     ChildRounding
		ChildBorderSize,     // float     ChildBorderSize
		PopupRounding,       // float     PopupRounding
		PopupBorderSize,     // float     PopupBorderSize
		FramePadding,        // ImVec2    FramePadding
		FrameRounding,       // float     FrameRounding
		FrameBorderSize,     // float     FrameBorderSize
		ItemSpacing,         // ImVec2    ItemSpacing
		ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
		IndentSpacing,       // float     IndentSpacing
		CellPadding,         // ImVec2    CellPadding
		ScrollbarSize,       // float     ScrollbarSize
		ScrollbarRounding,   // float     ScrollbarRounding
		GrabMinSize,         // float     GrabMinSize
		GrabRounding,        // float     GrabRounding
		TabRounding,         // float     TabRounding
		ButtonTextAlign,     // ImVec2    ButtonTextAlign
		SelectableTextAlign, // ImVec2    SelectableTextAlign
		COUNT
	};
	
	//* Taken From imgui.h
	enum class ImGuiWindowFlags: int {
		None                   = 0,
		NoTitleBar             = 1 << 0,   // Disable title-bar
		NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
		NoMove                 = 1 << 2,   // Disable user moving the window
		NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
		NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
		AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
		NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
		NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
		NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
		MenuBar                = 1 << 10,  // Has a menu-bar
		HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
		NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
		AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
		AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
		NoNavInputs            = 1 << 18,  // No gamepad/keyboard navigation within the window
		NoNavFocus             = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		UnsavedDocument        = 1 << 20,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
		NoDocking              = 1 << 21,  // Disable docking of this window

		NoNav                  = NoNavInputs | NoNavFocus,
		NoDecoration           = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
		NoInputs               = NoMouseInputs | NoNavInputs | NoNavFocus,
	};

	//* Taken from imgui.h
	enum class ImGuiCol: int {
		Text,
		TextDisabled,
		WindowBg,              // Background of normal windows
		ChildBg,               // Background of child windows
		PopupBg,               // Background of popups, menus, tooltips windows
		Border,
		BorderShadow,
		FrameBg,               // Background of checkbox, radio button, plot, slider, text input
		FrameBgHovered,
		FrameBgActive,
		TitleBg,
		TitleBgActive,
		TitleBgCollapsed,
		MenuBarBg,
		ScrollbarBg,
		ScrollbarGrab,
		ScrollbarGrabHovered,
		ScrollbarGrabActive,
		CheckMark,
		SliderGrab,
		SliderGrabActive,
		Button,
		ButtonHovered,
		ButtonActive,
		Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		HeaderHovered,
		HeaderActive,
		Separator,
		SeparatorHovered,
		SeparatorActive,
		ResizeGrip,
		ResizeGripHovered,
		ResizeGripActive,
		Tab,
		TabHovered,
		TabActive,
		TabUnfocused,
		TabUnfocusedActive,
		DockingPreview,        // Preview overlay color when about to docking something
		DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
		PlotLines,
		PlotLinesHovered,
		PlotHistogram,
		PlotHistogramHovered,
		TableHeaderBg,         // Table header background
		TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
		TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
		TableRowBg,            // Table row background (even rows)
		TableRowBgAlt,         // Table row background (odd rows)
		TextSelectedBg,
		DragDropTarget,
		NavHighlight,          // Gamepad/keyboard: current highlighted item
		NavWindowingHighlight, // Highlight window when using CTRL+TAB
		NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
		COUNT
	};

	void Init(void* WindowHandle);
	void ShutDown();

	void BeginFrame();
	void EndFrame();

	void Begin(const char* name, bool* p_open = nullptr, int flags = (int)ImGuiWindowFlags::None);
	void End();

	void Image(void* tex, Vector2 size);
	void Text(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

	bool Button(const char *label);
	bool ImageButton(void* tex, Vector2 size);

	void ListBox(const char* label, int* currentItem, const char* const *items, int count);
	void PlotLines(const char* label, const float* values, int valuesCount);
	
	bool BeginMenuBar();
	void EndMenuBar();
	
	bool BeginMainMenuBar();
	void EndMainMenuBar();

	bool BeginMenu(const char *label);
	void EndMenu();

	bool MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);

	bool IsCurrentWindowVisible();
	bool IsCurrentWindowFocusedAndHovered();

	void SetCursorPosX(float pos);
	void SetCursorPosY(float pos);

	void SameLine(float xOffset = 0.0f, float spacing = -1.0f);

	Vector4f* GetStyleColors();

	Vector2 GetWindowSize();

	void PushStyleVar(ImGuiStyleVar idx, float value);
	void PushStyleVar(ImGuiStyleVar idx, Vector2f value);

	void PopStyleVar(int n = 1);

	void PushStyleColor(ImGuiCol idx, Vector4 value);
	void PopStyleColor(int n = 1);

	Vector2 GetWindowContentRegionMin();
	Vector2 GetWindowContentRegionMax();

	Vector2 GetContentRegionAvail();

	void SetCursorPos(Vector2 pos);

	void ShowMetricsWindow();

	inline ImVec2 ToImVec2(Vector2f v) {
		return {v.x, v.y};
	}

	inline Vector2f FromImVec2(ImVec2 v) {
		return {v.x, v.y};
	}
}

#endif
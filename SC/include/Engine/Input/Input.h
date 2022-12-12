#pragma once

#include "Engine/Math/Math.h"
#include "KeyCode.h"

namespace SC
{
	namespace Editor {
		class EditorAddon;
	}

	class Input
	{
	public:
		static bool GetKeyDown(KeyCode key);
		static bool GetKeyUp(KeyCode key);
		static bool GetKey(KeyCode key);

		static Vector2 GetMousePos();
		
		static bool GetMouse(int n);
		static bool GetMouseDown(int n);
		static bool GetMouseUp(int n);
		
		static void SetState(bool _state);
	private:
		static bool state;

		static bool GetKeyDown_UnBlocked(KeyCode key);
		static bool GetKeyUp_UnBlocked(KeyCode key);
		static bool GetKey_UnBlocked(KeyCode key);

		Input() = delete;
		friend class Editor::EditorAddon;
	};
};

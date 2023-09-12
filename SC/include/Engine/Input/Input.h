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
	private:
		
		Input() = delete;
		friend class Editor::EditorAddon;
	};
};

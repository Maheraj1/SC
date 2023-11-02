#pragma once

#include "Engine/Core/Event.h"
#include "Engine/Math/Math.h"
#include "KeyCode.h"

namespace SC
{
	namespace Editor {
		class EditorAddon;
	}

	struct OnKeyDownArgs {
		KeyCode key;
		uint modifiers;
	};

	struct OnMouseButtonDownArgs {
		MouseButton key;
		uint modifiers;
	};

	class Input
	{
	public:
		static void Init();

		static bool GetKeyDown(KeyCode key);
		static bool GetKeyUp(KeyCode key);
		static bool GetKey(KeyCode key);

		static Vector2 GetMousePos();
		
		static bool GetMouse(int n);
		static bool GetMouseDown(int n);
		static bool GetMouseUp(int n);

		static EventHandler<OnKeyDownArgs> OnKeyDown;
		static EventHandler<OnMouseButtonDownArgs> OnMouseButtonDown;
	private:
		
		Input() = delete;
		friend class Editor::EditorAddon;
	};
};

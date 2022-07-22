#pragma once

#include "Engine/Core/Math.h"
#include "KeyCode.h"

namespace SC
{
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
		Input() { }
	};
};

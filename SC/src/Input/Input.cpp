#include "Engine/Input/Input.h"

namespace SC {
	bool Input::state = true;

	bool Input::GetKeyDown(KeyCode key) {
		if (!state) return false;
		return GetKeyDown_UnBlocked(key);
	}

	bool Input::GetKeyUp(KeyCode key) {
		if (!state) return false;
		return GetKeyUp_UnBlocked(key);
	}

	bool Input::GetKey(KeyCode key) {
		if (!state) return false;
		return GetKey_UnBlocked(key);
	}
}

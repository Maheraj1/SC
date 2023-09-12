#include "Engine/Core/Application.h"
#include "Engine/Input/Input.h"

#include <GLFW/glfw3.h>
#include <cstddef>

namespace SC 
{
	static GLFWwindow* window;

	bool Input::GetKeyDown(KeyCode key) {
		if (window == NULL) window = static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
		int k = glfwGetKey(window, (int)key);
		if (k == GLFW_PRESS) return true;
		return false;
	}

	bool Input::GetKeyUp(KeyCode key) {
		if (window == NULL) window = static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
		int k = glfwGetKey(static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow()), (int)key);
		if (k == GLFW_RELEASE) return true;
		return false;
	}

	bool Input::GetKey(KeyCode key) {
		if (window == NULL) window = static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
		int k = glfwGetKey(static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow()), (int)key);
		if (k == GLFW_PRESS || k == GLFW_REPEAT) return true;
		return false;
	}

	Vector2f Input::GetMousePos() {
		if (window == NULL) window = static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		float xf = static_cast<float>(x);
		float yf = static_cast<float>(y);
		return Vector2f(xf, yf);
	}

	bool Input::GetMouse(int n) {
		int status = glfwGetMouseButton(window, n);
		if (status == GLFW_PRESS || status == GLFW_REPEAT) return true;
		return false;
	}

	bool Input::GetMouseDown(int n) {
		int status = glfwGetMouseButton(window, n);
		if (status == GLFW_PRESS) return true;
		return false;
	}

	bool Input::GetMouseUp(int n) {
		int status = glfwGetMouseButton(window, n);
		if (status == GLFW_RELEASE) return true;
		return false;
	}
}
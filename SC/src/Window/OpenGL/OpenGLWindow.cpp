#include "Engine/Core/Platform.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Time.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/Window.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Debug/Debug.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace SC
{
	static uint8_t s_windowInstanceCount = 0;

	void Window::SetVSync(bool enabled)
	{
		m_data.VSync = enabled;
		if (enabled) Time::targetFps = glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
		else Time::targetFps = 60;
	}

	bool Window::IsVSync()
	{
		return m_data.VSync;
	}

	void* Window::GetNativeWindow()
	{
		return m_window;
	}

	Window::~Window()
	{
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_window));
		s_windowInstanceCount--;
		if (s_windowInstanceCount <= 0)
		{
			glfwTerminate();
		}
	}

	Window::Window(const WindowProps& props)
	{
		#ifdef SC_GL_VER
			int OpenGLVersion[2] = {SC_GL_VER};
		#else
			int OpenGLVersion[2] = {4, 1}; // version 4.1 to maintain opengl on macos support until metal gets implemented
		#endif
		if (s_windowInstanceCount == 0)
		{
			if (!glfwInit())
			{
				Debug::Error("Couldn't Initialize GLFW", "Window::GLFWInitializationError");
			}
		}

		m_data = props;

		glfwSetErrorCallback([](int error_code, const char* description) {
			Debug::Error(description, std::to_string(error_code));
		});

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLVersion[0]);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLVersion[1]);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef SC_PLATFORM_OSX
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		GLFWwindow* window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (window == NULL)
		{
			Debug::Error("Failed to create OpenGL Window (version: " + std::to_string(OpenGLVersion[0]) + '.' + std::to_string(OpenGLVersion[1]) + " Core", "Window::OpenGLWindowCreationError");
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Debug::Error("Failed to initialize Glad", "Window::OpenGLContextLoadError");
		}

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			Application::Get()->OnWindowClose.Call({});
		});

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
			glViewport(0, 0, width, height);
		});

		glfwSetWindowAspectRatio(window, 16, 9);

		Debug::Info((std::string)"(OpenGL Version): " + (const char*)glGetString(GL_VERSION));
		Debug::Info((std::string)"(OpenGL Renderer): " + (const char*)glGetString(GL_RENDERER));

		Internal::Renderer::Init();
		s_windowInstanceCount++;
		m_window = window;
		glfw_window = window;
	}

	Vector2i Window::GetWindowSize()
	{
		int sizeX;
		int sizeY;
		glfwGetFramebufferSize(glfw_window, &sizeX, &sizeY);

		return Vector2i(sizeX, sizeY);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		Internal::Renderer::Render();

		glfwSwapBuffers(glfw_window);
	}
}
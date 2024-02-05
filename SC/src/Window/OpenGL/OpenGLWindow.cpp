#include "Engine/Core/Platform.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Time.h"
#include "Engine/ECS/Camera.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Window/Window.h"
#include "Engine/Window/Window.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Debug/Debug.h"

#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
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
		else Time::targetFps = 100;
	}

	bool Window::IsVSync()
	{
		return m_data.VSync;
	}

	void* Window::GetNativeWindow()
	{
		return glfw_window;
	}

	Window::~Window()
	{
		glfwDestroyWindow(glfw_window);
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
				return;
			}
		}

		m_data = props;

		glfwSetErrorCallback([](int error_code, const char* description) {
			Debug::Error(description, std::to_string(error_code));
		});

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef SC_PLATFORM_OSX
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		GLFWwindow* window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (window == NULL)
		{
			Debug::Error("Failed to create OpenGL Window (version: " + std::to_string(OpenGLVersion[0]) + '.' + std::to_string(OpenGLVersion[1]) + " Core", "Window::OpenGLWindowCreationError");
			return;
		}

		Debug::Info("Created OpenGL Window", "SC::OpenGLWidnow");

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Debug::Error("Failed to initialize Glad", "Window::OpenGLContextLoadingError");
			return;
		}

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			Application::Get()->OnWindowClose.Call({});
		});

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
			glViewport(0, 0, width, height);
			#ifdef SC_RUNTIME
			Internal::Renderer::ViewPortSize = {width, height};
			#endif
		});

		glfwSetWindowAspectRatio(window, 16, 9);

		glfwSwapInterval(0);

		Debug::Info((std::string)"OpenGL Version - " + (const char*)glGetString(GL_VERSION), "SC::OpenGLWindow");
		Debug::Info((std::string)"OpenGL Renderer - " + (const char*)glGetString(GL_RENDERER), "SC::OpenGLWindow");

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

		auto app = Application::Get();
		
		for(auto addon: app->addons)
		{
			addon->PreFrameRender();
		}

		SceneManager::GetCurrentScene().GetCurrentCamera()->Render();

		for(auto addon: app->addons)
		{
			addon->PostFrameRender();
		}

		glfwSwapBuffers(glfw_window);
	}

	Matrix4 Window::GetScreenCoords()
	{
		int width, height;
		glfwGetWindowSize(glfw_window, &width, &height);
		return glm::ortho(0, width, 0, height);
	}

	void Window::SetWindowSize(Vector2i size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void Window::Clear(Color clear_color)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
	}
}
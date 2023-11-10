#define NO_USING_NAMESPACE

#include "Engine/Core/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/glm.hpp"
#include "Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include "Engine/Window/Window.h"
#include "Engine/ECS/Camera.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace SC {
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
		return m_window;
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

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		GLFWwindow* window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (window == NULL)
		{
			Debug::Error("Failed to create Glfw Window", "Window::GlfwWindowCreationError");
			return;
		}

		NSWindow* ns_window  = glfwGetCocoaWindow(window);


		Debug::Info("Created OpenGL Window", "SC::OpenGLWidnow");

		glfwMakeContextCurrent(window);
		
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			Application::Get()->OnWindowClose.Call({});
		});

		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
			// glViewport(0, 0, width, height);
			#ifdef SC_RUNTIME
			Internal::Renderer::ViewPortSize = {width, height};
			#endif
		});

		glfwSetWindowAspectRatio(window, 16, 9);

		glfwSwapInterval(0);

		// Debug::Info((std::string)"OpenGL Version - " + (const char*)glGetString(GL_VERSION), "SC::OpenGLWindow");
		// Debug::Info((std::string)"OpenGL Renderer - " + (const char*)glGetString(GL_RENDERER), "SC::OpenGLWindow");

		Internal::Renderer::Init();
		s_windowInstanceCount++;
		m_window = (__bridge_retained void*)ns_window;
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
		Clear();

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
		
	}

	void Window::Clear()
	{
		
	}
}
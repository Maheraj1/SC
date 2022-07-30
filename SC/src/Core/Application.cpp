#include "Engine/Core/Application.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include "Engine/Renderer/Renderer.h"
#include <thread>
#include <future>

namespace SC
{
	Application* Application::s_instance = nullptr;
	bool Application::AutoGenerateTexture = true;
	AppSettings::AppSettings(WindowProps windowProperties)
	{
		this->WindowProperties = windowProperties;
	}

	void Application::Run()
	{
		Running = true;

		// SceneSerializer::SerializeText("Scenes/test.yaml");
		if (AutoGenerateTexture) for (auto& [name, tex]: Resources::m_textures) tex.Generate();
		SceneManager::GetCurrentScene().Start();

		#ifdef SC_USE_THREADS
		std::thread RenderThread = std::thread(Internal::Renderer::StartBatch, &SceneManager::GetCurrentScene().m_objs);
		#endif

		while (Running)
		{	
			Internal::Renderer::RenderT = true;
			Time::Update();
			SceneManager::GetCurrentScene().Update();
			#ifdef SC_USE_THREADS
			if (!Internal::Renderer::done)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1us);
			}
			#else
			Internal::Renderer::StartBatch(&SceneManager::GetCurrentScene().m_objs);
			#endif
			window.OnUpdate();
			SceneManager::GetCurrentScene().CleanFrame();
		}
		Internal::Renderer::Run = false;

		Debug::Info("Closing Application", "SC::Application");
	}

	Application::Application(AppSettings settings)
	:window(settings.WindowProperties)
	{
		s_instance = this;
		OnWindowClose.AddListener([&](WindowCloseArgs args) {Application::Close();});
	}

	Application::~Application() { }

	void Application::Close()
	{
		s_instance->Running = false;
	}

	Application* Application::Get()
	{
		return s_instance;
	}

	Window& Application::GetWindow()
	{
		return s_instance->window;
	}

	void CloseApp()
	{
		Application::Close();
	}
}
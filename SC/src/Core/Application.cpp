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

		// std::future<void> RenderThread;

		while (Running)
		{	
			// RenderThread = std::async(std::launch::async, Internal::Renderer::StartBatch, &SceneManager::GetCurrentScene().m_objs);
			Time::Update();
			SceneManager::GetCurrentScene().Update();
			Internal::Renderer::StartBatch(&SceneManager::GetCurrentScene().m_objs);
			// RenderThread.wait();
			window.OnUpdate();
			SceneManager::GetCurrentScene().CleanFrame();
		}
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
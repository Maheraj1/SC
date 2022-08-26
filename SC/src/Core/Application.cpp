#include "Engine/Core/Application.h"
#include "Engine/Core/Math.h"
#include "Engine/ECS/Camera.h"
#include "Engine/ECS/Script.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Debug/Timmer.h"
#include "Engine/Core/Time.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Base.h"
#include "Engine/ECS/Entity.h"

#include "b2_world.h"
#include "b2_body.h"

#include <chrono>
#include <thread>
#include <future>

namespace SC
{
	Application* Application::s_instance = nullptr;
	bool Application::AutoGenerateTexture = true;
	bool Application::PlayerLoopStarted = false;
	
	AppSettings::AppSettings(WindowProps windowProperties)
	{
		this->WindowProperties = windowProperties;
	}

	void Application::Run(void(*func)(void))
	{
		Running = true;

		SC_REGISTER_COMPONENT(RigidBody);
		SC_REGISTER_COMPONENT(BoxCollider);
		SC_REGISTER_COMPONENT(CircleCollider);
		SC_REGISTER_COMPONENT(Camera);
		SC_REGISTER_COMPONENT(SpriteRenderer);

		Resources::AddTexture("Square");
		SceneSerializer::Init();


		Physics::Init();
		Debug::Info("Initalized Physics Engine", "SC::Application");
		
		if (AutoGenerateTexture) for (auto& [name, tex]: Resources::m_textures) tex.Generate();
		
		func();

		SceneManager::GetCurrentScene().Awake();
		SceneManager::GetCurrentScene().Start();

		PlayerLoopStarted = true;
		Debug::Info("Starting PlayerLoop", "SC::Application");

		while (Running)
		{
			Time::Update();
			
			Internal::Renderer::StartBatch(&SceneManager::GetCurrentScene().m_objs);
			window.OnUpdate();
			SceneManager::GetCurrentScene().CleanFrame();
		}
		Internal::Renderer::Run = false;

		Debug::Info("Closing Application", "SC::Application");
		Physics::ShutDown();
		Debug::Info("Stopped Physics Engine", "SC::Application");
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
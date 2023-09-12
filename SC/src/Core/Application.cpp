#include "Engine/Core/Application.h"
#include "Engine/Core/Event.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Camera.h"
#include "Engine/ECS/IScript.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Debug/Timmer.h"
#include "Engine/Core/Time.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Base.h"
#include "Engine/ECS/Entity.h"

#include "Engine/Scripting/ScriptEngine.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"

#include <chrono>
#include <cstddef>
#include <thread>
#include <future>

SC_REGISTER_RESOURCE(SC::Shader)
SC_REGISTER_RESOURCE(SC::Texture)

namespace SC
{
	Application* Application::s_instance = nullptr;
	bool Application::AutoGenerateTexture = true;
	bool Application::PlayerLoopStarted = false;

	bool Application::_IsFocused;
	bool Application::_IsEditor;
	bool Application::_EditMode;

	const bool& Application::IsFocused = Application::_IsFocused;
	const bool& Application::IsEditor = Application::_IsEditor;
	const bool& Application::EditMode = Application::_EditMode;
	
	AppSettings::AppSettings(WindowProps windowProperties)
	{
		this->WindowProperties = windowProperties;
	}

	void Application::Run(void(*func)(void)) // func is the PreAppRun function
	{
		Running = true;

		Resources::LoadFileResources("Assets");
		Resources::AddResource<Texture>("DefaultSquare")->Generate();

		SceneSerializer::Init();

		if (!(_IsEditor && _EditMode))
			Physics::Init();

		Debug::Info("Initalized Physics Engine", "SC::Application");
		
		if (AutoGenerateTexture) for (auto& [name, tex]: ResourceMap<Texture>::data) tex.Generate();
		Internal::ComponentData::RegisterAllComponents();
		
		func();
		

		for (auto addon: addons)
		{
			addon->Start();
		}

		if (!(IsEditor && EditMode))
		{
			SceneManager::GetCurrentScene().Awake();
			SceneManager::GetCurrentScene().Start();
		}
		

		PlayerLoopStarted = true;
		Debug::Info("Starting PlayerLoop", "SC::Application");

		while (Running)
		{
			for (int i = 0; i < OnFrameBeginFunctions.size(); i++) {
				OnFrameBeginFunctions[i]();
			}
			OnFrameBeginFunctions.clear();

			for (auto&& addon: addons)
			{
				addon->Update();
			}
			
			if (!IsEditor || (!EditMode && IsEditor))
			{
				SceneManager::GetCurrentScene().Update();
				Physics::UpdateData();
				Time::FixedUpdate();
			}
			
			Internal::Renderer::StartBatch();
			window.OnUpdate();
			
			SceneManager::GetCurrentScene().CleanFrame();
			Time::Update();

			for (int i = 0; i < OnFrameEndFunctions.size(); i++) {
				OnFrameEndFunctions[i]();
			}
			OnFrameEndFunctions.clear();
		}

		Debug::Info("Closing Application", "SC::Application");
		Physics::ShutDown();
		Debug::Info("Stopped Physics Engine", "SC::Application");

		SC_CLEAR_RESOURCE_DATA(Texture);
		SC_CLEAR_RESOURCE_DATA(Shader);
	}

	Application::Application(AppSettings settings)
	:window(settings.WindowProperties)
	{
		s_instance = this;
		OnWindowClose += [&](WindowCloseArgs args) {Application::Close();};
		OnAppPlay.SetListener([&](EventArgs a) { Application::OnScenePlay(); }, 0);
		OnAppStop.SetListener([&](EventArgs a) { Application::OnSceneStop(); }, 0);
		Scripting::ScriptEngine::Init("SC-JIT-Runtime", ".");
	}

	Application::~Application() { 
		Scripting::ScriptEngine::ShutDown();
	}

	void Application::Close()
	{
		s_instance->Running = false;
		Scripting::ScriptEngine::ShutDown();
	}

	Application* Application::Get()
	{
		return s_instance;
	}

	Window& Application::GetWindow()
	{
		return s_instance->window;
	}

	void Application::OnScenePlay() {
		_EditMode = false;

		auto scene = SceneManager::GetCurrentScene();
		
		Physics::Init();
		scene.Awake();
		scene.Start();
	}

	void Application::OnSceneStop() {
		_EditMode = true;
	}

	void Application::AddEndOfFrameFunction(std::function<void ()> func) {
		Get()->OnFrameEndFunctions.push_back(func);
	}

	void Application::AddStartOfFrameFunction(std::function<void ()> func) {
		Get()->OnFrameBeginFunctions.push_back(func);
	}

	void CloseApp()
	{
		Application::Close();
	}
}
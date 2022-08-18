#pragma once

#include <iostream>
#include <string>

#include "Engine/Core/Core.h"
#include "Engine/Core/Event.h"
#include "Engine/Window/Window.h"
#include "Engine/Scene/Scene.h"

#pragma exit SC::CloseApp

namespace SC
{
	void CloseApp();
	struct AppSettings
	{
		WindowProps WindowProperties;

		AppSettings(WindowProps windowProperties = WindowProps());
	};

	/**
	 * @brief Application class for main loop and logic
	 */
	class SC_API Application
	{
	private:
		static Application* s_instance;

		Window window;
		bool Running;

	public:
		struct WindowCloseArgs { };

		EventHandler<WindowCloseArgs> OnWindowClose;
		static bool AutoGenerateTexture;

	public:
		/**
		 * @brief Create Application instance
		 * 
		 * @param settings Settings for Application
		 */
		Application(AppSettings settings = AppSettings());

		~Application();

		/**
		 * @brief Runs The Application
		 * 
		 */
		void Run(void(*func)(void));

		/**
		 * @brief Close Window
		 */
		static void Close();

		static Window& GetWindow();

		/**
		 * @brief Get Application instance
		 * 
		 * @return Application*
		 */
		static Application* Get();
	};
};
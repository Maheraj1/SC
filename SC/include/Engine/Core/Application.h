#pragma once

#include <iostream>
#include <string>

#include "Engine/Core/Core.h"
#include "Engine/Core/Event.h"
#include "Engine/Internal/ApplicationAddons.h"
#include "Engine/Window/Window.h"
#include "Engine/Scene/Scene.h"

namespace SC
{
	namespace Editor { 
		class EditorAddon;
	}
	void CloseApp();
	struct AppSettings
	{
		WindowProps WindowProperties;
		const char* sceneFile;

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

		static bool _IsFocused;
		static bool _IsEditor;
		static bool _EditMode;

	public:
		struct WindowCloseArgs { };

		EventHandler<WindowCloseArgs> OnWindowClose;
		static bool AutoGenerateTexture;
		static bool PlayerLoopStarted;
		std::vector<ApplicationAddons*> addons;

		static const bool& IsFocused;
		static const bool& IsEditor;
		static const bool& EditMode;

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
		 */
		void Run(void(*func)(void));

		/**
		 * @brief Close Window
		 */
		static void Close();

		/**
		 * @brief Get the current window
		 * 
		 * @return Window& 
		 */
		static Window& GetWindow();

		/**
		 * @brief Get Application instance
		 * 
		 * @return Application*
		 */
		static Application* Get();

		/**
		 * @brief Used to set Edit or Run mode
		 * 
		 * @param EditMode 
		 */
		static void SetRunState(bool EditMode);
	friend class SC::Editor::EditorAddon;
	friend class SC::Window;
	};
};
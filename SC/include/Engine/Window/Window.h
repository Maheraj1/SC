#pragma once

#include "Engine/Core/Core.h"
#include <inttypes.h>
#include <string>

#include "Engine/Core/Math.h"
struct GLFWwindow;

namespace SC
{
	struct WindowProps
	{
		uint32_t width;
		uint32_t height;

		std::string title;

		bool VSync;

		WindowProps(uint32_t _width, uint32_t _height, std::string _title, bool _VSync)
		:width(_width), height(_height), title(_title), VSync(_VSync)
		{ }

		WindowProps()
		:width(1280), height(720), title("SC Engine"), VSync(false)
		{ }
	};

	class SC_API Window
	{
		public:
			Window(const WindowProps& props);
			~Window();

			void OnUpdate();
			void* GetNativeWindow();

			Vector2i GetWindowSize();

			void SetVSync(bool enabled);
			bool IsVSync();
		private:
			WindowProps m_data;
			void* m_window;
			GLFWwindow* glfw_window;
	};
}

#pragma once

#include "Engine/Core/Core.h"
#include <inttypes.h>
#include <string>

#include "Engine/Math/Math.h"

struct GLFWwindow;

namespace SC
{
	namespace Internal { class SceneRenderer; }

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
		:width(2240), height(1260), title("Scandium Engine"), VSync(false)
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
			void SetWindowSize(Vector2i size);

			void SetVSync(bool enabled);
			Matrix4 GetScreenCoords();
			bool IsVSync();
		private:
			void Clear();
		private:
			WindowProps m_data;
			void* m_window;
			GLFWwindow* glfw_window;
		friend class Camera;
		friend class Internal::SceneRenderer;
	};
}

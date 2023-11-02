#pragma once

#include "Engine/Input/Input.h"
#include "Engine/Input/KeyCode.h"

namespace SC
{
	class ApplicationAddon
	{
		public:
			ApplicationAddon() {}
			virtual ~ApplicationAddon() {}
			
			virtual void OnKeyDown(OnKeyDownArgs args) = 0;
			virtual void OnMouseButtonDown(OnMouseButtonDownArgs args) = 0;
			virtual void PreFrameRender() = 0;
			virtual void PostFrameRender() = 0;
			virtual void Update() = 0;
			virtual void Start() = 0;
			virtual void ShutDown() = 0;
	};
}
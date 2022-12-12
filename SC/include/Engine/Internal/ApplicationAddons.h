#pragma once

#include "Engine/Input/KeyCode.h"

namespace SC
{
	class ApplicationAddons
	{
		public:
			ApplicationAddons() {}
			virtual ~ApplicationAddons() {}
			virtual void PreFrameRender() = 0;
			virtual void PostFrameRender() = 0;
			virtual void Update() = 0;
			virtual void Start() = 0;
			virtual void ShutDown() = 0;
	};
}
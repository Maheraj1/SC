#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/Math.h"

struct b2World;

namespace SC {

	class RigidBody;

	class SC_API Physics
	{
		public:
			static float FixedTimeStep;
			static Vector2f gravity;
		private:
			static void Init();
			static void Update();
			static void ShutDown();
			static b2World& GetWorld();
		friend class Application;
		friend class RigidBody;
		friend class Time;
	};
}

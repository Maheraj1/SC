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
			static Vector2f gravity;
			static b2World* GetPhysicsWorld();
		private:
			static void Init();
			static void Update();
			static void ShutDown();
			static void Step(float dt);
			static void UpdateData();
			static void UpdatePositions();
			static std::vector<RigidBody*> rigidBodies;
			static b2World* world;
		
		friend class Application;
		friend class RigidBody;
		friend class Collider;
		friend class Time;
		friend class Window;
		friend class SceneSerializer;
	};
}

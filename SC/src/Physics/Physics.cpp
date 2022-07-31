#include "Engine/Physics/Physics.h"
#include "Engine/Core/Math.h"
#include "b2_math.h"
#include "b2_world.h"
#include "box2d.h"

namespace SC {
	// Class static variables
	float Physics::FixedTimeStep = .5f;
	Vector2f Physics::gravity = Vector2f(0.0f, -9.81);

	// File Static variables
	static b2World world(ToBox2dVector2(Physics::gravity));

	void Physics::Init()
	{
		world = b2World(ToBox2dVector2(Physics::gravity));
	}

	void Physics::ShutDown() { }

	b2World& Physics::GetWorld()
	{
		return world;
	}
}

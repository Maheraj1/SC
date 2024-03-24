#pragma once

#include "glm/glm.hpp"
#include <initializer_list>

namespace SC::A {

	struct Vector2: public glm::vec2 {

		Vector2();
		Vector2(float n);
		Vector2(float x, float y);
		Vector2(std::initializer_list<float> params);
		~Vector2() = default;

		static float GetSqMagnitude(Vector2 vec);
		static float GetMagnitude(Vector2 vec);
		static float GetDistance(Vector2 a, Vector2 b);

		void Normalise();
		static Vector2 Normalised(Vector2 vec);
	};
}

#include "Engine/Math/Vector2.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>

namespace SC::A {

	// Float based Vector2
	Vector2::Vector2()
	:glm::vec2() 
	{ }

	Vector2::Vector2(float n)
	:glm::vec2(n, n)
	{ }

	Vector2::Vector2(float x, float y)
	:glm::vec2(x, y)
	{ }

	Vector2::Vector2(std::initializer_list<float> params)
	:glm::vec2(*params.begin(), *params.begin()+1)
	{ }

	float Vector2::GetSqMagnitude(Vector2 vec) {
		return powf(vec.x, 2) + powf(vec.x, 2);
	}

	float Vector2::GetMagnitude(Vector2 vec) {
		return sqrt(GetSqMagnitude(vec));
	}

	float Vector2::GetDistance(Vector2 a, Vector2 b) {
		return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
	}

	Vector2 Vector2::Normalised(Vector2 vec) {
		float d = GetMagnitude(vec);
		return {vec.x / d, vec.y / d};
	}

	void Vector2::Normalise() {
		*this = Normalised(*this);
	}
}
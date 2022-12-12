#include "Engine/Math/Vector2.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>

namespace SC::A {

	// Float based Vector2
	template<>
	Vector2f::Vector2()
	:x(0), y(0) 
	{ }

	template<>
	Vector2f::Vector2(float n)
	:x(n), y(n) 
	{ }

	template<>
	Vector2f::Vector2(float x, float y)
	:x(x), y(y)
	{ }

	template<>
	Vector2f::Vector2(std::initializer_list<float> params)
	:x(*params.begin()), y(*params.begin()+1)
	{ }

	template<>
	float Vector2f::GetSqMagnitude(Vector2f vec) {
		return powf(vec.x, 2) + powf(vec.x, 2);
	}

	template<>
	float Vector2f::GetMagnitude(Vector2f vec) {
		return sqrt(GetSqMagnitude(vec));
	}

	template<>
	float Vector2f::GetDistance(Vector2f a, Vector2f b) {
		return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
	}

	template<>
	Vector2f Vector2f::Normalised(Vector2f vec) {
		float d = GetMagnitude(vec);
		return {vec.x / d, vec.y / d};
	}

	template<>
	void Vector2f::Normalise() {
		*this = Normalised(*this);
	}

	template<>
	Vector2f Vector2f::operator+(const Vector2f&& other) {
		return {this->x + other.x, this->y + other.y};
	}

	template<>
	Vector2f Vector2f::operator-(const Vector2f&& other) {
		return {this->x - other.x, this->y - other.y};
	}

	template<>
	Vector2f Vector2f::operator==(const Vector2f&& other) {
		return (this->x == other.x) && (this->y == other.y);
	}

	template<>
	Vector2f Vector2f::operator!=(const Vector2f&& other) {
		return (this->x == other.x) && (this->y == other.y);
	}

	template<>
	Vector2f Vector2f::operator*(float other) {
		return {this->x * other, this->y * other};
	}

	template<>	
	Vector2f Vector2f::operator/(float other) {
		return {this->x / other, this->y / other};
	}

	// Int based Vector2
	template<>
	Vector2i::Vector2()
	:x(0), y(0) 
	{ }

	template<>
	Vector2i::Vector2(int n)
	:x(n), y(n) 
	{ }

	template<>
	Vector2i::Vector2(int x, int y)
	:x(x), y(y)
	{ }

	template<>
	Vector2i::Vector2(std::initializer_list<int> params)
	:x(*params.begin()), y(*params.begin()+1)
	{ }

	template<>
	float Vector2i::GetDistance(Vector2i a, Vector2i b) {
		return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
	}

	template<>
	float Vector2i::GetSqMagnitude(Vector2i vec) = delete;

	template<>
	float Vector2i::GetMagnitude(Vector2i vec) = delete;

	template<>
	Vector2i Vector2i::operator+(const Vector2i&& other) {
		return {this->x + other.x, this->y + other.y};
	}

	template<>
	Vector2i Vector2i::operator-(const Vector2i&& other) {
		return {this->x - other.x, this->y - other.y};
	}

	template<>
	Vector2i Vector2i::operator==(const Vector2i&& other) {
		return (this->x == other.x) && (this->y == other.y);
	}

	template<>
	Vector2i Vector2i::operator!=(const Vector2i&& other) {
		return (this->x == other.x) && (this->y == other.y);
	}

	template<> Vector2i Vector2i::operator*(float other) = delete;
	template<> Vector2i Vector2i::operator/(float other) = delete;
}
#pragma once

#include <initializer_list>
namespace SC::A {
	
	template<typename T = float>
	struct Vector2 {
		T x, y;

		Vector2();
		Vector2(T n);
		Vector2(T x, T y);
		Vector2(std::initializer_list<T> params);
		~Vector2() = default;

		static float GetSqMagnitude(Vector2 vec);
		static float GetMagnitude(Vector2 vec);
		static float GetDistance(Vector2 a, Vector2 b);

		void Normalise();
		static Vector2 Normalised(Vector2 vec);

		Vector2 operator+(const Vector2&& other);
		Vector2 operator-(const Vector2&& other);

		Vector2 operator==(const Vector2&& other);
		Vector2 operator!=(const Vector2&& other);

		Vector2 operator*(float other);
		Vector2 operator/(float other);
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;
}

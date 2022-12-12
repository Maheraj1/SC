#include "Engine/Math/Color.h"

namespace SC::A {
	Color::Color(uint8_t r, uint8_t g, uint8_t b)
	:r(r), g(g), b(b)
	{ }

	Color::Color(std::initializer_list<uint8_t> colorChannels)
	:r(*colorChannels.begin()), g(*colorChannels.begin()), b(*colorChannels.begin())
	{ }
}
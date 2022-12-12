#pragma once

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>
#include <initializer_list>

namespace SC::A {
	struct Color {
		uint8_t r, g, b;

		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(std::initializer_list<uint8_t> colorChannels);
		~Color() = default;
	};
}

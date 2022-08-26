#pragma once

#include "Engine/Core/Common.h"
#include "Engine/Core/Core.h"
#include <string>
#include <utility>

namespace SC {
	struct Entity;
	class SC_API SCObject
	{
	public:
		const char* name;
		SCObject(const char* name = Common::EmptyString):name(name) {}
	};
}

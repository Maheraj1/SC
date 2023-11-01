#pragma once

#include "Engine/Core/Common.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"
#include <string>
#include <utility>

namespace SC {
	class Entity;
	class SC_API SCObject
	{
	public:
		std::string name = Common::EmptyString;
		SCObject(std::string name = Common::EmptyString):name(name) {}

		UUID uuid;
	};
}

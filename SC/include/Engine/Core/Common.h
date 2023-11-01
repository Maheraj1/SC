#pragma once

#include "Engine/Core/Core.h"

namespace SC {
	struct Material;
	struct SC_API Common
	{
		static const char* EmptyCString;
		static std::string EmptyString;
		static Material* TempMaterial;
	};
}

#include "Engine/Core/Common.h"

#include "Engine/Renderer/Material.h"

namespace SC {
	const char* Common::EmptyCString = "";
	
	std::string Common::EmptyString = std::string("");

	Material* Common::TempMaterial = new Material();
}

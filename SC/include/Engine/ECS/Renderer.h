#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/Script.h"

namespace SC
{
	class Texture;
	class Shader;
	struct Renderer: public Script {
		Texture* texture;
		Shader* shader;
		Color color;

		void PostRender() { }
	};
}
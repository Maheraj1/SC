#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/NativeScript.h"

namespace SC
{
	class Texture;
	class Shader;
	struct Renderer: public NativeScript {
		Texture* texture;
		Shader* shader;
		Color color;

		virtual void PostRender() = 0;
	};
}
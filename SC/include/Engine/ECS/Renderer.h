#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/NativeScript.h"
#include "Engine/Renderer/Material.h"

namespace SC
{
	class Texture;
	class Shader;
	struct Renderer: public NativeScript {
		Renderer(Material* mat) :mat(mat) { }
		Material* mat;
		Color blendColor;

		virtual void PostRender() = 0;
	};
}
#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Math.h"

namespace SC
{
	class Entity;
	namespace Internal { 
		class Renderer;
		struct BatchData;
		void RenderBatch(const std::vector<Entity>* objs);
	}
	class SC_API Transform
	{
	private:
		Matrix4 GetModel(bool useScale = true) const;
	public:
		Transform();
		Transform(Vector2 position, float rotation, Vector2 scale);

		Vector2 position;
		float rotation;
		Vector2 scale;

		friend class Camera;
		friend class Internal::Renderer;
		friend void Internal::RenderBatch(const std::vector<Entity>* objs);
	};
}
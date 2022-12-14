#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"
#include "Engine/Serialization/SerializableObject.h"

namespace SC
{
	class Entity;
	namespace Internal { 
		class Renderer;
	}

	/**
	 * @brief A class to handle matrix calculations and positioning, rotating and scaling of objects
	 * 
	 */
	class SC_API Transform: Serialization::SerializableObject
	{
	private:
		Matrix4 GetModel(bool useScale = true) const;
		void Serialize() const override;
		void DeSerialize() override;
	public:
		Transform();
		Transform(Vector2 position, float rotation, Vector2 scale);

		Vector2 position;
		float rotation;
		Vector2 scale;

		friend class Camera;
		friend class Internal::Renderer;
		friend class SceneSerializer;
	};
}
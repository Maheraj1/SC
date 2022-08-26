#include "Engine/ECS/Transform.h"
#include "Engine/Core/Math.h"
#include "Engine/Serialization/SerializedData.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace SC
{
	Matrix4 Transform::GetModel(bool useScale) const
	{
		Matrix4 model(1.0f);
		model = glm::translate(model, Vector3f(position.x, position.y, 1.0f));
		model = glm::rotate(model, glm::radians(rotation), Vector3f(0.0f, 0.0f, 1.0f));
		if (useScale)
			model = glm::scale(model, Vector3f(scale.x, scale.y, 1.0f));
		return model;
	}

	Transform::Transform()
	:position(0.0f, 0.0f), rotation(0.0f), scale(1.0f, 1.0f)
	{ }

	Transform::Transform(Vector2 position, float rotation, Vector2 scale)
	:position(position), rotation(0), scale(scale)
	{ }

	void Transform::Serialize() const
	{
		SC_ADD_PARAMETER(position);
		SC_ADD_PARAMETER(rotation);
		SC_ADD_PARAMETER(scale);
	}

	void Transform::DeSerialize()
	{
		SC_GET_PARAMETER(position);
		SC_GET_PARAMETER(rotation);
		SC_GET_PARAMETER(scale);
	}
}
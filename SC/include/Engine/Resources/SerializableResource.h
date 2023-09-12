#pragma once

#include "Engine/Resources/Resource.h"
#include "Engine/Serialization/SerializableObject.h"

namespace SC {
	class SerializableResource: public Resource, public Serialization::SerializableObject {
	public:
		SerializableResource() = default;
		~SerializableResource() = default;

		virtual bool IsSerializable() const override { return true; }
	
		virtual void Serialize() const override = 0;
		virtual void DeSerialize() override = 0;
	};
}
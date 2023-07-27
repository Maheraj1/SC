#pragma once

#include "Engine/Core/Buffer.h"
#include "Engine/Serialization/SerializableObject.h"

namespace SC::Scripting {
	struct SerializableField: public Serialization::SerializableObject {
		std::string name;
		std::string type;
		Buffer data;

		void Serialize() const override;

		void DeSerialize() override;
	};
}
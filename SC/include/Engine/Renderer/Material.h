#pragma once

#include "Engine/Math/Math.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Serialization/ResourceReference.h"
#include "Engine/Serialization/SerializableObject.h"

namespace SC {
	struct Material: public SerializableResource {
		Material();
		Material(const char* fp);
		~Material() = default;
		
		Color color;
		Shader shader;
		Texture texture;

		void Init();

		virtual uint64_t GetID() const override { return uuid; }
		virtual void Delete() override;
	
		virtual void Serialize() const override;
		virtual void DeSerialize() override;
	protected:

		std::string fp;
	};
}
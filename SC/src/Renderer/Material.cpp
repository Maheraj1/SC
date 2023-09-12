#include "Engine/Renderer/Material.h"
#include "Engine/Serialization/SerializedData.h"

namespace SC {

	Material::Material() 
	:shader(nullptr), texture(nullptr) { }

	Material::Material(const char* fp)
	:fp(fp) 
	{ }

	void Material::Serialize() const {
		SC_ADD_PARAM((Color16)color, "color");

		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

	void Material::DeSerialize() {
		Color16 color = {255, 255, 255};
		SC_GET_PARAM(color, "color");
		this->color = (Color)color;

		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

}
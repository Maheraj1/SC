#include "Engine/Renderer/Material.h"
#include "Engine/Core/SCObject.h"
#include "Engine/Serialization/SerializedData.h"

namespace SC {

	Material::Material() 
	:shader(nullptr), texture(nullptr) { }

	Material::Material(std::string fp)
	:fp(fp)  { }

	Material::~Material() { }

	void Material::Delete() { }

	void Material::Serialize() const {
		SC_ADD_PARAM((Color16)color, "Color");

		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_ADD_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

	void Material::DeSerialize() {
		Color16 color = {255, 255, 255};
		SC_GET_PARAM(color, "Color");
		this->color = (Color)color;

		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->shader, "Shader");
		SC_GET_PARAM<SerializableObj>((SerializableObj*)&this->texture, "Texture");
	}

	#ifdef SC_EDITOR_IMPL

	void Material::OnIGUI(Editor::EditorDrawData& dcmd) { 
		dcmd.DrawColor(color/255.0f, "Color");
	}

	void Material::PostIGUI(Editor::EditorDrawData& dcmd) { 
		color = (*((ColorF*)(dcmd.data[0].data))) * 255.0f;
	}


	#endif

}
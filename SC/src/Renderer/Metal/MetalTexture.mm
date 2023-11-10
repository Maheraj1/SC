#define NO_USING_NAMESPACE

#include "Engine/Renderer/Texture.h"

namespace SC {
	Texture::Texture() : m_id(0) { }

	Texture::Texture(const char* fp)
	:m_id(0),
	fp(fp)
	{ }

	Texture::~Texture() {
		if (Valid) Delete();
	}

	void Texture::Delete() { }

	void Texture::Generate() { }

	void Texture::SetAttribute(TextureParameters param, TextureProperties value) { }

	void Texture::Bind(unsigned int slot) { }

	void Texture::SetData(unsigned char* data, unsigned int size) { }

	uint Texture::GetTextureID() const { return m_id; }
	
	void* Texture::GetTexture() const { return reinterpret_cast<void*>(m_id); }
	
	uint64_t Texture::GetID() const { return uuid; }
}
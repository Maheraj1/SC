#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"
#include "Engine/Core/Base.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/Resources.h"

#include "Other/Sprites/Square.h"
#include "glad/glad.h"

#include "stb/stb_image.h"

namespace SC 
{
	Texture::Texture()
	:fp(nullptr), m_id(0),
	TextureF{GL_LINEAR, GL_LINEAR}, TextureW{GL_REPEAT, GL_REPEAT},
	Format(GL_RGB8), IFormat(GL_RGB)
	{ }

	Texture::~Texture()
	{
		if (m_id == 0) return;
		if (m_id == Resources::currentTexture) Resources::currentTexture = 0;
		glDeleteTextures(1, &m_id);
	}

	void Texture::Generate()
	{
		bool alpha = false;
		if (IFormat == GL_RGBA) alpha = true;

		GLCall(glGenTextures(1, &m_id));

		int width, height, nrChannels;
		unsigned char* data;

		if (fp != nullptr)
		{
			data = stbi_load(fp, &width, &height, &nrChannels, 3 + (int)alpha); // if alpha is true then it will have int value of 1
		} else 
		{
			data = (unsigned char*)&white_square_data;
		}

		if (data == nullptr) return;

		GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, IFormat, GL_UNSIGNED_BYTE, data));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	void Texture::SetAttribute(TextureParameters param, TextureProperties value)
	{
		if (param == TextureParameters::TextureWarpX)
		{
			if (value == TextureProperties::WrapRepeat) TextureW[0] = GL_REPEAT;
			if (value == TextureProperties::WrapMirroredRepeat) TextureW[0] = GL_MIRRORED_REPEAT;
			if (value == TextureProperties::WrapClampToEdge) TextureW[0] = GL_CLAMP_TO_EDGE;
			if (value == TextureProperties::WrapClampToBorder) TextureW[0] = GL_CLAMP_TO_BORDER;
		} else if (param == TextureParameters::TextureWarpY)
		{
			if (value == TextureProperties::WrapRepeat) TextureW[1] = GL_REPEAT;
			if (value == TextureProperties::WrapMirroredRepeat) TextureW[1] = GL_MIRRORED_REPEAT;
			if (value == TextureProperties::WrapClampToEdge) TextureW[1] = GL_CLAMP_TO_EDGE;
			if (value == TextureProperties::WrapClampToBorder) TextureW[1] = GL_CLAMP_TO_BORDER;
		} else if (param == TextureParameters::TextureWarpBoth)
		{
			if (value == TextureProperties::WrapRepeat) {TextureW[0] = GL_REPEAT; TextureW[1] = GL_REPEAT;}
			if (value == TextureProperties::WrapMirroredRepeat) {TextureW[0] = GL_MIRRORED_REPEAT; TextureW[1] = GL_MIRRORED_REPEAT;}
			if (value == TextureProperties::WrapClampToEdge) {TextureW[0] = GL_CLAMP_TO_EDGE; TextureW[1] = GL_CLAMP_TO_EDGE;}
			if (value == TextureProperties::WrapClampToBorder) {TextureW[0] = GL_CLAMP_TO_BORDER; TextureW[1] = GL_CLAMP_TO_BORDER;}
		}

		if (param == TextureParameters::TextureFilterMin)
		{
			if (value == TextureProperties::FilterLinear) TextureW[0] = GL_LINEAR;
			if (value == TextureProperties::FilterNearest) TextureW[0] = GL_NEAREST;
		} else if (param == TextureParameters::TextureFilterMag)
		{
			if (value == TextureProperties::FilterLinear) TextureW[1] = GL_LINEAR;
			if (value == TextureProperties::FilterNearest) TextureW[1] = GL_NEAREST;
		} else if(param == TextureParameters::TextureFilterBoth)
		{
			if (value == TextureProperties::FilterLinear) {TextureW[0] = GL_LINEAR; TextureW[1] = GL_LINEAR;}
			if (value == TextureProperties::FilterNearest) {TextureW[0] = GL_NEAREST; TextureW[1] = GL_NEAREST;}
		}

		if (param == TextureParameters::Format)
		{
			if (value == TextureProperties::FormatRGB)
			{
				Format = GL_RGB8;
				IFormat = GL_RGB;
			} else if (value == TextureProperties::FormatRGBA) {
				Format = GL_RGBA8;
				IFormat = GL_RGBA;
			}
		}
	}

	void Texture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	uint Texture::GetTextureID() { return m_id; }
}

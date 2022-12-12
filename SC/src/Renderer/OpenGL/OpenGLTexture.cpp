#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"
#include "Engine/Core/Base.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Resources/Resources.h"

#include "glad/glad.h"

#include "stb/stb_image.h"

namespace SC 
{
	Texture::Texture()
	:m_id(0),
	TextureW{GL_REPEAT, GL_REPEAT}, TextureF{GL_LINEAR, GL_LINEAR},
	Format(GL_RGB8), IFormat(GL_RGB),
	fp(nullptr)
	{ }

	Texture::Texture(const char* fp)
	:m_id(0),
	TextureW{GL_REPEAT, GL_REPEAT}, TextureF{GL_LINEAR, GL_LINEAR},
	Format(GL_RGB8), IFormat(GL_RGB),
	fp(fp)
	{ }

	Texture::~Texture()
	{
		if (Valid) Delete();
	}

	void Texture::Delete()
	{
		Valid = false;
		if (m_id == 0) return;
		glDeleteTextures(1, &m_id);
	}

	void Texture::Generate()
	{
		Valid = true;
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
			data = nullptr;
		}

		if (data != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, IFormat, GL_UNSIGNED_BYTE, data));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glBindTexture(GL_TEXTURE_2D, 0);
		}else {
			uint dat = 0xffffffff;
			GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &dat));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (data != nullptr)
			stbi_image_free(data);
	}

	void Texture::SetAttribute(TextureParameters param, TextureProperties value)
	{
		if ((uint)param & (uint)TextureParameters::TextureWarpX)
		{
			if ((uint)value & (uint)TextureProperties::WrapRepeat) TextureW[0] = GL_REPEAT;
			if ((uint)value & (uint)TextureProperties::WrapMirroredRepeat) TextureW[0] = GL_MIRRORED_REPEAT;
			if ((uint)value & (uint)TextureProperties::WrapClampToEdge) TextureW[0] = GL_CLAMP_TO_EDGE;
			if ((uint)value & (uint)TextureProperties::WrapClampToBorder) TextureW[0] = GL_CLAMP_TO_BORDER;
		} 
		if ((uint)param & (uint)TextureParameters::TextureWarpY)
		{
			if ((uint)value & (uint)TextureProperties::WrapRepeat) TextureW[1] = GL_REPEAT;
			if ((uint)value & (uint)TextureProperties::WrapMirroredRepeat) TextureW[1] = GL_MIRRORED_REPEAT;
			if ((uint)value & (uint)TextureProperties::WrapClampToEdge) TextureW[1] = GL_CLAMP_TO_EDGE;
			if ((uint)value & (uint)TextureProperties::WrapClampToBorder) TextureW[1] = GL_CLAMP_TO_BORDER;
		}

		if ((uint)param & (uint)TextureParameters::TextureFilterMin)
		{
			if ((uint)value & (uint)TextureProperties::FilterLinear) TextureW[0] = GL_LINEAR;
			if ((uint)value & (uint)TextureProperties::FilterNearest) TextureW[0] = GL_NEAREST;
		} if ((uint)param & (uint)TextureParameters::TextureFilterMag)
		{
			if ((uint)value & (uint)TextureProperties::FilterLinear) TextureW[1] = GL_LINEAR;
			if ((uint)value & (uint)TextureProperties::FilterNearest) TextureW[1] = GL_NEAREST;
		}

		if ((uint)param & (uint)TextureParameters::Format)
		{
			if ((uint)value & (uint)TextureProperties::FormatRGB)
			{
				Format = GL_RGB8;
				IFormat = GL_RGB;
			} else if ((uint)value == (uint)TextureProperties::FormatRGBA) {
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

	void Texture::SetData(unsigned char* data, unsigned int size)
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint Texture::GetTextureID() const { return m_id; }
	
	uint64_t Texture::GetID() const { return uuid; }
}

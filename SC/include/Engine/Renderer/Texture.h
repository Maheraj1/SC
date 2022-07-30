#pragma once

#include "Engine/Core/Core.h"

namespace SC {
	enum class TextureProperties
	{
		None = -1,
		
		WrapRepeat,
		WrapMirroredRepeat,
		
		WrapClampToEdge,
		WrapClampToBorder,

		FilterNearest,
		FilterLinear,

		FormatRGB,
		FormatRGBA,
	};

	enum class TextureParameters
	{
		None = -1,

		TextureWarpX,
		TextureWarpY,
		TextureWarpBoth,

		TextureFilterMin,
		TextureFilterMag,
		TextureFilterBoth,

		Format,
	};

	class SC_API Texture
	{
	private:
		unsigned int m_id;
		unsigned int TextureW[2];
		unsigned int TextureF[2];
		unsigned int Format;
		unsigned int IFormat;

		const char* fp;
	public:
		Texture();
		~Texture();

		void Generate();
		unsigned int GetTextureID();
		void SetAttribute(TextureParameters param, TextureProperties value);
		void Bind(unsigned int slot = 0);
		void SetData(unsigned char* data, unsigned int size);

		friend class Resources;
	};
}
#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Resources/Resource.h"

namespace SC {
	enum class TextureProperties: unsigned int
	{
		WrapRepeat = 1 << 1,
		WrapMirroredRepeat = 1 << 2,
		
		WrapClampToEdge = 1 << 3,
		WrapClampToBorder = 1 << 4,

		FilterNearest = 1 << 5,
		FilterLinear = 1 << 6,

		FormatRGB = 1 << 7,
		FormatRGBA = 1 << 8,
	};

	enum class TextureParameters: unsigned int
	{
		TextureWarpX = 1 << 1,
		TextureWarpY = 1 << 2,
		TextureWarpBoth = TextureWarpX | TextureWarpY,

		TextureFilterMin = 1 << 3,
		TextureFilterMag = 1 << 4,
		TextureFilterBoth = TextureFilterMag | TextureFilterMin,

		Format = 1 << 5,
	};

	class SC_API Texture: public Resource
	{
	private:
		unsigned int m_id = 0;
		unsigned int TextureW[2];
		unsigned int TextureF[2];
		unsigned int Format;
		unsigned int IFormat;

		const char* fp;

		bool Valid;
	public:
		Texture();
		Texture(const char* fp);
		~Texture();

		virtual uint64_t GetID() const override;

		void Generate();
		void Delete() override;

		unsigned int GetTextureID() const;
		void SetAttribute(TextureParameters param, TextureProperties value);
		void Bind(unsigned int slot = 0);
		void SetData(unsigned char* data, unsigned int size);

		friend class Resources;
	};
}
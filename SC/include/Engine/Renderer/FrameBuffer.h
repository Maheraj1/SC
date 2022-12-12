#pragma once

#include "Engine/Math/Math.h"
#include <_types/_uint32_t.h>

namespace SC
{
	class FrameBuffer
	{
	private:
		uint32_t w, h;
		uint32_t RendererId;
		uint32_t tex;
		uint32_t col;
		bool multiSample;
		uint32_t samples;
	public:
		FrameBuffer();
		FrameBuffer(uint32_t w, uint32_t h, bool multiSample=false, uint32_t samples=1);
		~FrameBuffer();

		void Init();

		uint32_t Invalidate();
		uint32_t GetID();
		uint32_t GetTextureID();

		void AddColorAttachment();
		
		void Bind();
		void UnBind();

		void Resize(Vector2i size);
	};
}
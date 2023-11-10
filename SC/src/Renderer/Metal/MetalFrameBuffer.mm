#define NO_USING_NAMESPACE

#include "Engine/Renderer/FrameBuffer.h"

namespace SC {
	void FrameBuffer::Bind() { }

	void FrameBuffer::UnBind() { }

	uint32_t FrameBuffer::GetID()
	{
		return RendererId;
	}

	uint32_t FrameBuffer::GetTextureID()
	{
		return tex;
	}

	FrameBuffer::FrameBuffer(uint32_t w, uint32_t h, bool multiSample, uint32_t samples)
	:w(w), h(h), multiSample(multiSample), samples(samples)
	{ }

	FrameBuffer::FrameBuffer()
	:w(1280), h(720), multiSample(false), samples(1)
	{ }

	FrameBuffer::~FrameBuffer()
	{
		
	}

	void FrameBuffer::Init()
	{
		Invalidate();
	}

	uint32_t FrameBuffer::Invalidate()
	{
		return 0;
	}

	void FrameBuffer::Resize(Vector2i size)
	{
		w = size.x;
		h = size.y;
		Invalidate();
	}
}

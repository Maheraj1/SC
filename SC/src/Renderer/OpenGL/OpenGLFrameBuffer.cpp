#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/Core/Base.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"

#include "glad/glad.h"

#include "stb/stb_image.h"

namespace SC
{
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RendererId);
	}

	void FrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

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
		glDeleteFramebuffers(1, &RendererId);
		glDeleteTextures(1, &tex);
	}

	void FrameBuffer::Init()
	{
		Invalidate();
	}

	uint32_t FrameBuffer::Invalidate()
	{
		if(RendererId)
		{
			glDeleteFramebuffers(1, &RendererId);
			glDeleteTextures(1, &tex);
		}
		GLCall(glGenTextures(1, &tex));
		GLCall(glBindTexture(GL_TEXTURE_2D, tex));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			
		GLCall(glGenFramebuffers(1, &RendererId));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, RendererId));

		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0));

		SC_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		return tex;
	}

	void FrameBuffer::Resize(Vector2i size)
	{
		w = size.x;
		h = size.y;
		Invalidate();
	}
}
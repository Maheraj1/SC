#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Base.h"

#ifdef SC_DEBUG
#define GLCall(x) SC::Internal::OpenGL::GLClearErrors();\
x;\
SC_ASSERT(SC::Internal::OpenGL::GLLogCall(#x, __FILE__, __LINE__))
#else
	#define GLCall(x) x
#endif

namespace SC::Internal::OpenGL
{
	SC_API const char* GLEnumToString(unsigned int error);
	SC_API void GLClearErrors();
	SC_API bool GLLogCall(const char* func, const char* filename, int line);
}

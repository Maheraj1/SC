#include "Engine/Renderer/OpenGL/OpenGLErrors.h"
#include "Engine/Core/Base.h"

#include <glad/glad.h>

namespace SC::Internal::OpenGL {
	void GLClearErrors()
	{
		#ifdef SC_DEBUG
		while (glGetError() != GL_NO_ERROR);
		#endif
	}

	bool GLLogCall(const char* func, const char* filename, int line)
	{
		#if SC_DEBUG
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			char hexDat[7];
			sprintf(hexDat, "0x%04X", err);
			Debug::Error((std::string)GLEnumToString(err) + '/' + hexDat + '/' + std::to_string(err) + ' ' + func + ' ' + filename + ':' + std::to_string(line), "OpenGL Error");
			return false;
		}
		#endif
		return true;
	}
	#ifdef SC_DEBUG
	const char* GLEnumToString(unsigned int error)
	{
		if (error == GL_INVALID_ENUM)		return "Invalid Enum";
		if (error == GL_INVALID_VALUE)		return "Invalid Value";
		if (error == GL_INVALID_OPERATION)	return "Invalid Operation";
		else								return "UNKNOWN";
	}
	#endif
}

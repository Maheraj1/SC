#define NO_USING_NAMESPACE

#include "Engine/Renderer/Shader.h"

namespace SC {
	Shader::Shader(const char* src):src(src) { }

	Shader::~Shader() {
		if (Valid) Delete();
	}

	void Shader::Delete() { }

	unsigned int Shader::GetShaderID() const { return m_id; }

	void Shader::Bind() const { }

	uint64_t Shader::GetID() const
	{
		return uuid;
	}

	bool Shader::Compile(const char* src) { return true; }
}
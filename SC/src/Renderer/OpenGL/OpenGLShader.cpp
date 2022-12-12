#include "Engine/Debug/Debug.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"

#include "glad/glad.h"

#include <sstream>
#include <string>

namespace SC
{
	// Main things

	Shader::Shader(const char* src):src(src) { }

	Shader::~Shader()
	{
		if (Valid) Delete();
	}

	void Shader::Delete()
	{
		Valid = false;
		if (m_id == 0) return;
		glDeleteProgram(m_id);
		m_id = 0;
	}

	unsigned int Shader::GetShaderID() const { return m_id; }

	void Shader::Bind() const
	{
		glUseProgram(m_id);
	}

	uint64_t Shader::GetID() const
	{
		return uuid;
	}

	bool Shader::Compile(const char* src)
	{
		Valid = true;
		const char* _src = nullptr;
		
		if (this->src == nullptr) _src = src;
		else _src = this->src;
		
		std::string vss_s;
		std::string fss_s;

		// set this in a scope to destroy variables after the variables are not needed
		{
			std::string line;
			std::istringstream ss(_src);
			int8_t mode = -1;

			while (std::getline(ss, line))
			{
				if (line == "#shader vert")
				{
					mode = 0;
				} 
				else if (line == "#shader frag")
				{
					mode = 1;
				} else
				{
					if (mode == 0) vss_s += line + '\n';
					else if (mode == 1) fss_s += line + '\n';
				}
			}
		}

		const char* vss = vss_s.c_str();
		const char* fss = fss_s.c_str();

		// Create Shader

		uint vs = glCreateShader(GL_VERTEX_SHADER);
		uint fs = glCreateShader(GL_FRAGMENT_SHADER);
		int success;

		// Compile vertex Shader
		
		glShaderSource(vs, 1, &vss, NULL);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			int size;
			glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &size);
			char* buffer = (char*)alloca(size + 1);
			glGetShaderInfoLog(vs, size, NULL, buffer);

			Debug::Error((std::string)"Failed to compile vertex shader, OpenGL Output: " + buffer, "Shader::Compile::VertexShaderCompilationError");
			m_id = Internal::Renderer::ErrorShaderID;
			return false;
		}

		// Compile fragment shader

		glShaderSource(fs, 1, &fss, NULL);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			int size;
			glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &size);
			char* buffer = (char*)alloca(size + 1);
			glGetShaderInfoLog(fs, size, NULL, buffer);

			Debug::Error((std::string)"Failed to compile fragment shader, OpenGL Output: " + buffer, "Shader::Compile::FragmentShaderCompilationError");
			return false;
		}

		// Create OpenGL Program and link it to the Shaders

		this->m_id = glCreateProgram();
		glAttachShader(m_id, vs);
		glAttachShader(m_id, fs);
		glValidateProgram(m_id);
		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			int size;
			glGetProgramiv(vs, GL_INFO_LOG_LENGTH, &size);
			char* buffer = (char*)alloca(size + 1);
			glGetProgramInfoLog(vs, size, NULL, buffer);
			buffer[size] = 0;

			Debug::Error((std::string)"Failed to link shaders, OpenGL Output: " + buffer, "Shader::Program::Link");
			m_id = Internal::Renderer::ErrorShaderID;
			return false;
		}

		// delete shaders because they are compiled and linked into the program and no linger needed
		glDeleteShader(vs);
		glDeleteShader(fs);
		return true;
	}

	// Shader variable stuff

	void Shader::SetFloat(const char* VarName, float value)
	{
		GLCall(glUniform1f(glGetUniformLocation(m_id, VarName), value));
	}

	void Shader::SetVector2f(const char* VarName, Vector2f vec)
	{
		GLCall(glUniform2f(glGetUniformLocation(m_id, VarName), vec.x, vec.y));
	}

	void Shader::SetVector3f(const char* VarName, Vector3f vec)
	{
		GLCall(glUniform3f(glGetUniformLocation(m_id, VarName), vec.x, vec.y, vec.z));
	}

	void Shader::SetVector4f(const char* VarName, Vector4f vec)
	{
		GLCall(glUniform4f(glGetUniformLocation(m_id, VarName), vec.x, vec.y, vec.z, vec.w));
	}

	void Shader::SetInt(const char* VarName, int value)
	{
		GLCall(glUniform1i(glGetUniformLocation(m_id, VarName), value));
	}

	void Shader::SetVector2i(const char* VarName, Vector2i vec)
	{
		GLCall(glUniform2i(glGetUniformLocation(m_id, VarName), vec.x, vec.y));
	}

	void Shader::SetVector3i(const char* VarName, Vector3i vec)
	{
		GLCall(glUniform3i(glGetUniformLocation(m_id, VarName), vec.x, vec.y, vec.z));
	}

	void Shader::SetVector4i(const char* VarName,Vector4i vec)
	{
		GLCall(glUniform4i(glGetUniformLocation(m_id, VarName), vec.x, vec.y, vec.z, vec.w));
	}
}

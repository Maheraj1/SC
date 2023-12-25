#include "Engine/Core/Base.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"
#include "Engine/Debug/Timmer.h"

#include "glm/fwd.hpp"
#include <cstddef>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <array>
#include <unordered_map>

namespace SC::Internal
{
	unsigned int Renderer::VAO;
	unsigned int Renderer::ErrorShaderID;

	bool Renderer::WireFrameMode = false;

	static uint VBO, EBO;
	static unsigned int UBO_MVP;

	static Matrix4 proj;

	struct BatchData {
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT> pos;
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT> texCoords;
	};

	void Renderer::Render()
	{
		// Vector4f pos = proj * Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
		if (WireFrameMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		for (const BatchQuad &quad_dat : quad_data)
		{
			if (quad_dat.count <= 0) continue;

			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_dat.data), &quad_dat.data));
			GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 24 * MAX_BATCH_COUNT, &quad_dat.indices[0]));
			
			GLCall(glUseProgram(quad_dat.shader->GetShaderID()));
			
			int32_t tex[MAX_TEXTURE_SLOT_USAGE] = {};

			for (int i = 0; i < quad_dat.textureCount; i++)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + i));
				GLCall(glBindTexture(GL_TEXTURE_2D, quad_dat.textures[i]->GetTextureID()));
				tex[i] = i;
			}

			GLCall(glUniform1iv(glGetUniformLocation(quad_dat.shader->GetShaderID(), "u_textures"), quad_dat.textureCount, tex));

			GLCall(glDrawElements(GL_TRIANGLES, 6 * quad_dat.count, GL_UNSIGNED_INT, 0));
		}

		
	}

	void Renderer::Init()
	{	
		// Create Uniform buffer for ModelViewProjection it will store the product of View and Projection
		glGenBuffers(1, &UBO_MVP);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_MVP);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4), NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_MVP, 0, sizeof(Matrix4));

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 4 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		// position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, position)));
		glEnableVertexAttribArray(0);
		
		// texCoords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, color)));
		glEnableVertexAttribArray(1);

		// color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, tex_coords)));
		glEnableVertexAttribArray(2);

		// texIndex
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(offsetof(VertexData, tex)));
		glEnableVertexAttribArray(3);

		/// Error Shader
		// Error Shader will be used if compiling of client shader failed using Shader::Compile()

		uint vs = glCreateShader(GL_VERTEX_SHADER);
		uint fs = glCreateShader(GL_FRAGMENT_SHADER);

		const char *vss = R"(#version 410 core
layout (location = 0) in vec2 i_pos;

layout(std140) uniform Matrix
{
	mat4 proj;
};

void main() {
	gl_Position = proj * vec4(i_pos.xy, 0.0, 1.0);
})";
		const char* fss = R"(#version 410 core
out vec4 o_color;

void main() {
	o_color = vec4()" ERROR_SHADER_COLOR_RGB R"();
})";

        glShaderSource(vs, 1, &vss, NULL);
		glShaderSource(fs, 1, &fss, NULL);
		glCompileShader(vs);
		glCompileShader(fs);

		ErrorShaderID = glCreateProgram();
		glAttachShader(ErrorShaderID, vs);
		glAttachShader(ErrorShaderID, fs);
		glLinkProgram(ErrorShaderID);

		glDeleteShader(vs);
		glDeleteShader(fs);

		// Hard coded texture shader
	 	Shader* shader = Resources::AddResource<Shader>("Default Sprite");
		shader->Compile(R"(#shader vert
#version 410 core

layout (location = 0) in vec2  i_pos;
layout (location = 1) in vec2  i_texCoords;
layout (location = 2) in vec3  i_color;
layout (location = 3) in float i_tex;

out VS_OUT {
	vec2 texCoords;
	vec3 color;
	float tex;
} vs_out;

layout(std140) uniform Matrix
{
	mat4 proj;
};

void main() {
	gl_Position = vec4(i_pos.xy, 0.0, 1.0) * proj;
	vs_out.texCoords = i_texCoords;
	vs_out.color = i_color;
	vs_out.tex = i_tex;
}
#shader frag
#version 410 core

out vec4 o_color;

in VS_OUT
{
	vec2 texCoords;
	vec3 color;
	float tex;
} fs_in;

uniform sampler2D u_textures[10];

void main() {
	int i = int(fs_in.tex);
	o_color = texture(u_textures[i], fs_in.texCoords) * vec4(fs_in.color, 1.0);
})");
		shader->uuid = 1;
	}

	void Renderer::SetMVP(Matrix4 _proj)
	{
		proj = _proj;
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, UBO_MVP));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), glm::value_ptr(_proj)));
	}

}
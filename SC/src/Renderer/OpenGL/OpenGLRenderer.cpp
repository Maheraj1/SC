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
	unsigned int Renderer::VAO_Line;
	unsigned int Renderer::VAO_Line_Strip;
	unsigned int Renderer::Current_VAO;
	unsigned int Renderer::ErrorShaderID;

	bool Renderer::WireFrameMode = false;

	static uint VBO, EBO;
	static unsigned int VBO_Line;
	static unsigned int VBO_Line_Strip;
	static unsigned int Current_VBO;
	static unsigned int UBO_MVP;

	static Matrix4 proj;

	struct BatchData {
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT> pos;
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT> texCoords;
	};

	void Renderer::Render()
	{
		if (WireFrameMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		/// Quad Rendering

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBindVertexArray(VAO));

		for (const BatchQuad &quad_dat : quad_data)
		{
			if (quad_dat.count <= 0) continue;

			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_dat.data), quad_dat.data.data()));
			GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(quad_dat.indices), quad_dat.indices.data()));
			
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

		quad_data.clear();

		/// Line Rendering

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO_Line));
		GLCall(glBindVertexArray(VAO_Line));

		for (const BatchLine& lines : line_data) {
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lines.data), lines.data.data()));
			GLCall(glUseProgram(lines.shader->GetShaderID()));

			GLCall(glDrawArrays(GL_LINES, 0, lines.count * 2));
		}
	}

	void Renderer::Init()
	{	
		// Create Uniform buffer for ModelViewProjection it will store the product of View and Projection
		glGenBuffers(1, &UBO_MVP);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_MVP);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4), NULL, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_MVP, 0, sizeof(Matrix4));
		
		/// Quad Buffers

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData_Quad) * 4 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		// position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_Quad), (void*)(offsetof(VertexData_Quad, position)));
		glEnableVertexAttribArray(0);
		
		// color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData_Quad), (void*)(offsetof(VertexData_Quad, color)));
		glEnableVertexAttribArray(1);

		// texCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_Quad), (void*)(offsetof(VertexData_Quad, tex_coords)));
		glEnableVertexAttribArray(2);

		// texIndex
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData_Quad), (void*)(offsetof(VertexData_Quad, tex)));
		glEnableVertexAttribArray(3);

		/// Line Buffers
		uint VAO_line[2] = {0, 0};

		glGenVertexArrays(2, VAO_line);

		VAO_Line = VAO_line[0];
		VAO_Line_Strip = VAO_line[1];

		//* Simple Lines
		glBindVertexArray(VAO_Line);
		
		glGenBuffers(1, &VBO_Line);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Line);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData_Line) * 2 * MAX_SINGLE_LINE_BATCH_SIZE, nullptr, GL_DYNAMIC_DRAW);

		// position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData_Line), (void*)(offsetof(VertexData_Line, position)));
		glEnableVertexAttribArray(0);
		
		// color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData_Line), (void*)(offsetof(VertexData_Line, color)));
		glEnableVertexAttribArray(1);

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
	 	Shader* shader_quad = Resources::AddResource<Shader>("Default Sprite");
		shader_quad->Compile(R"(#shader vert
#version 410 core

layout (location = 0) in vec2  i_pos;
layout (location = 1) in vec3  i_color;
layout (location = 2) in vec2  i_texCoords;
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
	gl_Position = proj * vec4(i_pos.xy, 0.0, 1.0);
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
		shader_quad->uuid = 1;
		
		// Hard Coded Line Shader
		Shader* shader_line = Resources::AddResource<Shader>("Default Line");
		shader_line->Compile(R"(#shader vert
#version 410 core

layout (location = 0) in vec2  i_pos;
layout (location = 1) in vec3  i_color;

out VS_OUT {
	vec3 color;
} vs_out;

layout(std140) uniform Matrix
{
	mat4 proj;
};

void main() {
	gl_Position = proj * vec4(i_pos.xy, 0.0, 1.0);
	vs_out.color = i_color;
}
#shader frag
#version 410 core

out vec4 o_color;

in VS_OUT
{
	vec3 color;
} fs_in;

void main() {
	o_color = vec4(fs_in.color, 1.0);
}
		)");
		shader_line->uuid = 2;
	}

	void Renderer::SetMVP(Matrix4 _proj)
	{
		proj = _proj;
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, UBO_MVP));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), glm::value_ptr(_proj)));
	}

}
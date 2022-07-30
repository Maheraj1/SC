#include "Engine/Core/Base.h"
#include "Engine/Core/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/OpenGL/OpenGLErrors.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Debug/MemoryTracker.h"

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
	// just to make code look better
	#define BatchEnt batches[currentBatch].entities

	unsigned int Renderer::VAO;
	unsigned int Renderer::ErrorShaderID;
	
	bool Renderer::done = false;
	bool Renderer::Run = true;
	bool Renderer::WireFrameMode = false;
	bool Renderer::RenderT = false;
	static bool* _RenderT = &Renderer::RenderT;
	static bool* _done = &Renderer::done;
	static bool* Run = &Renderer::Run;

	static uint VBO, EBO;
	static unsigned int UBO_MVP;
	static uint8_t currentMode = 1;

	static std::vector<BatchData> batchData;
	static Matrix4 proj;
	static const Matrix4* _proj_ = &proj;

	static uint texW;

	// using this variables to let the multithread function use it
	static const uint _MBC = Renderer::MAX_BATCH_COUNT;
	static const uint _MTS = Renderer::MAX_TEXTURE_SLOT_USAGE;

	void RenderBatch(const std::vector<Entity>* objs)
	{
		#if SC_USE_THREADS
		std::mutex m;
		std::lock_guard<std::mutex> lg(m);
		#endif
		std::vector<Batch> batches = {Batch()};

		uint currentBatch = 0;
		for (Entity ent: *objs)
		{
			if (batches[currentBatch].entities.entC == _MBC) currentBatch++;
			if (!ent.HasComponent<SpriteRenderer>()) continue;

			SpriteRenderer* sr = ent.GetComponentPtr<SpriteRenderer>();
			if (batches[currentBatch].shader != sr->shader->GetShaderID())
			{
				for (int i = 0; i < batches.size(); i++)
				{
				 	if (batches[i].shader == sr->shader->GetShaderID()) {
						currentBatch = i;
					} else if (batches[i].shader == 0)
					{
						batches[i].shader = sr->shader->GetShaderID();
					} else if (batches.size()-1 == currentBatch) {
						currentBatch = batches.size();
						batches.push_back(Batch());
						batches[currentBatch].shader = sr->shader->GetShaderID();
					}
				}
			}

			for (uint i = 0; i < _MTS; i++)
			{
				if (batches[currentBatch].Textures[i].GetID() == sr->texture->GetTextureID()) /* if texture list has the texture we need break from it add to binding list */
				{
					BatchEnt.textureIndex[BatchEnt.entC] = i;
					break;
				}
				else if (batches[currentBatch].Textures[i].GetID() == 0) /* if slot is empty we will use it */
				{
					batches[currentBatch].Textures[i].GetID() = sr->texture->GetTextureID();
					BatchEnt.textureIndex[BatchEnt.entC] = i;
					break;
				} else if (i == _MTS-1) /* if it is last slot and no empty slot is found create a new Batch */
				{
					currentBatch = batches.size();
					batches.push_back(Batch());
					batches[currentBatch].shader = sr->shader->GetShaderID();
					batches[currentBatch].Textures[0].GetID() = sr->texture->GetTextureID();
					BatchEnt.textureIndex[BatchEnt.entC] = i;
				}
			}
			batches[currentBatch].shader = sr->shader->GetShaderID();
			
			// Set all Vertex Positions
			BatchEnt.pos[BatchEnt.entC + 0] = (Vector2f)(Vector4f( 1.0f,  1.0f, 0.0, 1.0) * ent.transform.GetModel());
			BatchEnt.pos[BatchEnt.entC + 1] = (Vector2f)(Vector4f( 1.0f, -1.0f, 0.0, 1.0) * ent.transform.GetModel());
			BatchEnt.pos[BatchEnt.entC + 2] = (Vector2f)(Vector4f(-1.0f, -1.0f, 0.0, 1.0) * ent.transform.GetModel());
			BatchEnt.pos[BatchEnt.entC + 3] = (Vector2f)(Vector4f(-1.0f,  1.0f, 0.0, 1.0) * ent.transform.GetModel());

			// Set all Color Values
			for (int i = 0; i < 4; i++) BatchEnt.color[BatchEnt.entC + i] = ((Vector3i)sr->color)/255.0f;

			BatchEnt.entC++;
		}

		// batchData.reserve(batches.size());

		for (int i = 0; i < batches.size(); i++)
		{
			std::array<uint, _MBC * 6> ind;

			for (int v = 0; v * 6 < batches[i].entities.entC; v += 6) {
				ind[v + 0] = 0;
				ind[v + 1] = 1;
				ind[v + 2] = 3;

				ind[v + 3] = 1;
				ind[v + 4] = 2;
				ind[v + 5] = 3;
			}
			
			std::array<Vector2f, _MBC * 4> texCoords;

			for (int i2 = 0; i2 < batches[i].entities.entC; i2 += 4) {
				texCoords[i2 + 0] = {1, 1};
				texCoords[i2 + 1] = {1, 0};
				texCoords[i2 + 2] = {0, 0};
				texCoords[i2 + 3] = {0, 1};
			}

			batchData.emplace_back(batches[i].entities.pos,
									 texCoords,
									  batches[i].entities.color,
									  	batches[i].entities.textureIndex,
										 ind,
										  batches[i].Textures,
										   batches[i].shader,
											batches[i].entities.entC
									   );
		}
	}

	void Renderer::StartBatch(const std::vector<Entity> *objs)
	{
		RenderBatch(objs);
	}

	void Renderer::Render()
	{
		if (WireFrameMode && currentMode != 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			currentMode = 0;
		} else if (currentMode != 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		for (const BatchData &batch : batchData)
		{
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 32 * MAX_BATCH_COUNT, batch.pos.data()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 32 * MAX_BATCH_COUNT, 32 * MAX_BATCH_COUNT, batch.texCoords.data()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 64 * MAX_BATCH_COUNT, 48 * MAX_BATCH_COUNT, batch.color.data()));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 112 * MAX_BATCH_COUNT, 16 * MAX_BATCH_COUNT, batch.tex.data()));
			GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 24 * MAX_BATCH_COUNT, batch.indices.data()));
			
			// if (Resources::currentShader != batch.shader) {
				glUseProgram(batch.shader);
			// 	Resources::currentShader = batch.shader;
			// }
			
			int32_t tex[MAX_TEXTURE_SLOT_USAGE];

			for (int i = 0; i < MAX_TEXTURE_SLOT_USAGE; i++)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + i));
				GLCall(glBindTexture(GL_TEXTURE_2D, batch.Textures[i].TexID));
				tex[i] = i;
			}

			GLCall(glUniform1iv(glGetUniformLocation(batch.shader, "u_textures"), MAX_TEXTURE_SLOT_USAGE, tex));
			
			GLCall(glDrawElements(GL_TRIANGLES, 6 * batch.size, GL_UNSIGNED_INT, 0));
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 128 * MAX_BATCH_COUNT, nullptr, GL_DYNAMIC_DRAW);

		// position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
		glEnableVertexAttribArray(0);
		
		// texCoords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8, reinterpret_cast<void*>(32 * MAX_BATCH_COUNT)); // 8 Positions 4 bytes each = 32 bytes
		glEnableVertexAttribArray(1);

		// color
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12, reinterpret_cast<void*>(64 * MAX_BATCH_COUNT)); // 8 Coords 4 bytes each = 32 bytes + last (32 bytes) = 64
		glEnableVertexAttribArray(2);

		// texIndex
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4, reinterpret_cast<void*>(112 * MAX_BATCH_COUNT)); // 4 Colors 12 byte each = 48 bytes + last (64 bytes) = 112
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
		const char *fss = R"(#version 410 core
out vec4 o_color;

void main() {
	o_color = vec4(1, 0.75294117647, 0.79607843137, 1.0);
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

		// Hard coded solid color shader
		Resources::AddShader("SolidColor").Compile(R"(#shader vert
#version 410 core

layout (location = 0) in vec2 i_pos;
layout (location = 2) in vec3 i_color;

layout(std140) uniform Matrix
{
	mat4 proj;
};

out VS_OUT {
	vec3 color;
} vs_out;

void main() {
	gl_Position = proj * vec4(i_pos.xy, 0.0, 1.0);
	vs_out.color = i_color;
}
#shader frag
#version 410 core

out vec4 o_color;

in VS_OUT {
	vec3 color;
} fs_in;

void main() {
	o_color = vec4(fs_in.color, 1.0);
})");

		// Hard coded texture shader
		Resources::AddShader("Sprite").Compile(R"(#shader vert
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
	gl_Position = proj * vec4(i_pos.xy, 1.0, 1.0);
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
	}

	void Renderer::SetMVP(Matrix4 _proj)
	{
		proj = _proj;
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, UBO_MVP));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), glm::value_ptr(_proj)));
	}


	EntityBatchArray::EntityBatchArray() { }

	EntityBatchArray::~EntityBatchArray() { }

	void EntityBatchArray::operator++() { entC++;}

	void EntityBatchArray::operator--() { entC--;}

	BatchTextureConnection::BatchTextureConnection(uint TexID):TexID(TexID) { }
	BatchTextureConnection::~BatchTextureConnection() { }

	void BatchTextureConnection::AddEntity(uint64_t entID)
	{
		entIDs[lastIndex] = entID;
		lastIndex++;
	}
}
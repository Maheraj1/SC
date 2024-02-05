#pragma once

#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene/Scene.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"

//* Graphics API Batch Type
#ifndef SC_RENDERER_OpenGL
#define BATCH_TYPE_CLASS_ARRAY
#endif

#define ERROR_SHADER_COLOR_R "1, "
#define ERROR_SHADER_COLOR_G "0.75294117647, "
#define ERROR_SHADER_COLOR_B "0.79607843137"

#define ERROR_SHADER_COLOR_RGB ERROR_SHADER_COLOR_R ERROR_SHADER_COLOR_G ERROR_SHADER_COLOR_B

namespace SC::Internal
{
	struct CameraData;
	struct BatchQuad;

	struct ImQuad {
		Transform transform;
		Color color;
		Texture* texture;
	};

	class SC_API Renderer
	{
	public:
		static const uint MAX_BATCH_COUNT = 100;
		static const ushort MAX_TEXTURE_SLOT_USAGE = 10;
		
		static bool WireFrameMode;
		static bool IsFrameBuffer;

		static unsigned int VAO;
		static uint ErrorShaderID;

		static Vector2i Resolution;
		
		static void Init();
		static void Render();
		static void CleanUp();

		static void RenderQuad(ImQuad& quad, Shader* shader);
		static void RenderQuad(ImQuad& quad, uint64_t shader = 1);

		static void SetMVP(Matrix4 proj);

	private:
		static std::vector<BatchQuad> quad_data;
		Renderer() {}
	};
	// Raw Vertex data
	struct SC_API VertexData {
		Vector2f position;
		Color color;
		Vector2f tex_coords;
		uint tex;
	};

	struct SC_API BatchQuad {
		// 4 vertices of quad
		/*
		q1: v1, v2, v3, v4,
		q2: v5, v6, v7, v8,
		...
		*/
		std::array<VertexData, Renderer::MAX_BATCH_COUNT * 4> data;
		
		// 6 for 6 vertex in a quad
		std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices;
		
		std::array<Texture*, Renderer::MAX_TEXTURE_SLOT_USAGE> textures;

		Shader* shader;
		// count of number of quads
		uint count = 0;
		// count of number of textures
		ushort textureCount = 0;
	};
}

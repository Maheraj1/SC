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
	struct BatchLineStrip;
	struct BatchLine;

	struct ImQuad {
		Transform transform;
		Color color;
		Texture* texture;
	};

	struct ImLine {
		// both vectors must be of same size
		std::vector<Vector2f> vertices;
		std::vector<Color> color;
		
		// Only for stripped lines
		float thickness = 5.0f;
		
		// Are the vertices of same line
		bool IsStripped = false;
	};


	class SC_API Renderer
	{
	public:
		static const uint MAX_BATCH_COUNT = 100;
		static const ushort MAX_TEXTURE_SLOT_USAGE = 10;
		static const ushort MAX_LINE_SIZE = 50;
		static const ushort MAX_SINGLE_LINE_BATCH_SIZE = 50;
		
		static bool WireFrameMode;
		static bool IsFrameBuffer;

		static unsigned int VAO;
		static unsigned int VAO_Line_Strip;
		static unsigned int Current_VAO;
		static uint ErrorShaderID;

		static Vector2i Resolution;
		
		static void Init();
		static void Render();
		static void CleanUp();

		static void RenderQuad(ImQuad& quad, Shader* shader);
		static void RenderQuad(ImQuad& quad, uint64_t shader = 1);

		static void RenderLine(ImLine& line, Shader* shader);
		static void RenderLine(ImLine& line, uint64_t shader = 2);

		static void SetMVP(Matrix4 proj);

	private:
		static std::vector<BatchQuad> quad_data;
		static std::vector<BatchLine> line_data;
		static std::vector<BatchLineStrip> line_strip_data;
		Renderer() = default;
	};
	
	// Raw Vertex data
	struct SC_API VertexData_Quad {
		Vector2f position;
		Color color;
		Vector2f tex_coords;
		uint tex;
	};

	struct SC_API VertexData_Line {
		Vector2f position;
		Color color;
	};

	struct SC_API BatchQuad {
		// 4 vertices of quad
		/*
		q1: v1, v2, v3, v4,
		q2: v5, v6, v7, v8,
		...
		*/
		std::array<VertexData_Quad, Renderer::MAX_BATCH_COUNT * 4> data;
		
		// 6 for 6 vertex in a quad
		std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices;
		
		std::array<Texture*, Renderer::MAX_TEXTURE_SLOT_USAGE> textures;

		Shader* shader;
		// count of number of quads
		uint count = 0;
		// count of number of textures
		ushort textureCount = 0;
	};

	struct SC_API BatchLineStrip {
		// This will contain batches of lines
		/** We ain't sure of size of the line so,
			l1: v1, v2, v3 ... vn, 				- Batch Line 1
			l2: vn+1, vn+2, vn+2 ... vn+m, 		- Batch Line 2
			...

			We would use one Batch for EACH line & one separate for lines with only two points.
			This would use lines with dynamic length with a limit
		**/

		std::array<VertexData_Line, Renderer::MAX_LINE_SIZE> data;
		
		// Thickness of the line
		float thickness;

		Shader* shader;
		// count of number of points / vertices
		uint count = 0;
	};

	struct SC_API BatchLine {
		// This will contain batches of lines having two points
		/** 
			l1: v1, v2,
			l2: v3, v4,
			...

			Here, We would take pairs of vertices to be the line.
			This system is use full as this optimises debug or simple lines' rendering saving memory
		**/

		std::array<VertexData_Line, Renderer::MAX_SINGLE_LINE_BATCH_SIZE * 2> data;

		// Thickness of the line
		//* NOTE: This is fixed just for simplicity & can be changed globally
		static float thickness;

		Shader* shader;
		// count of number of points
		uint count = 0;
	};
}

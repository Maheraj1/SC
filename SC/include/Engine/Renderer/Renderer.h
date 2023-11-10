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

#ifdef BATCH_TYPE_CLASS_ARRAY
	#define T_Batch_Data EntityBatch
	#define F_Batch_Data BatchEntityData
	#define T_Batch_Data_Array std::array<T_Batch_Data, Renderer::MAX_BATCH_COUNT>
	#define F_Batch_Data_Array std::array<F_Batch_Data, Renderer::MAX_BATCH_COUNT>
#else
	#define T_Batch_Data EntityBatchArray
	#define F_Batch_Data BatchData
	#define T_Batch_Data_Array T_Batch_Data
	#define F_Batch_Data_Array F_Batch_Data
#endif

namespace SC::Internal
{
	class BatchEntityData;
	class BatchData;
	struct CameraData;


	struct ImQuad {
		Vector2 position;
		float rotation;
		Vector2 scale;
		Color color;
	};

	class SC_API Renderer
	{
	public:
		static const int MAX_BATCH_COUNT = 100;
		static const unsigned int MAX_TEXTURE_SLOT_USAGE = 10;
		
		static bool WireFrameMode;
		static bool IsFrameBuffer;

		static unsigned int VAO;
		static uint ErrorShaderID;

		static Vector2i Resolution;
		
		static void Init();
		static void Render();
		static void CleanUp();
		static void StartBatch();

		static void RenderQuad(Vector2 pos, float rotation, Vector2 scale, Color color);

		static void SetMVP(Matrix4 proj);

	private:
		static std::vector<F_Batch_Data_Array> batchData;
		static std::vector<ImQuad> quads;
		Renderer() {}
	};

	#ifdef BATCH_TYPE_CLASS_ARRAY
	class SC_API EntityBatch
	{
	public:
		EntityBatch() = default;
		~EntityBatch() = default;

		std::array<Vector4f, 4> pos;
		std::array<ColorF, 4> color;
		std::array<float, 4> textureIndex;
	};

	class SC_API BatchEntityData
	{
	public:
	
		BatchEntityData(std::array<Vector4f, 4> pos,
				  std::array<Vector2f, 4> texCoords,
				   std::array<ColorF, 4> color,
					std::array<float, 4> tex, 
					 std::array<uint, 6> indices,
		 			  std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures,
						uint shader,
						 uint size
						 )
		  :pos(pos), texCoords(texCoords), color(color), tex(tex), indices(indices), Textures(Textures), shader(shader), size(size)
		   { }
		
		~BatchEntityData() = default;

		std::array<Vector4f, 4> pos;
		std::array<Vector2f, 4> texCoords;
		std::array<ColorF, 4> color;
		std::array<float, 4> tex;
		std::array<uint, 6> indices;
		std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
		uint size;
		
	};


	#else

	class SC_API EntityBatchArray
	{
	public:
		EntityBatchArray() = default;
		~EntityBatchArray() = default;

		std::array<Vector4f, Renderer::MAX_BATCH_COUNT * 4> pos;
		std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color;
		std::array<float, Renderer::MAX_BATCH_COUNT * 4> textureIndex;
		uint32_t entC;
	};

	/**
	 * @brief A Batch Data that contains raw data about the batch
	 * 
	 */
	class SC_API BatchData
	{
	public:
	
		BatchData(std::array<Vector4f, Renderer::MAX_BATCH_COUNT * 4> pos,
				  std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> texCoords,
				   std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color,
					std::array<float, Renderer::MAX_BATCH_COUNT * 4> tex, 
					 std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices,
		 			  std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures,
						uint shader,
						 uint size
						)
		  :pos(pos), texCoords(texCoords), color(color), tex(tex), indices(indices), Textures(Textures), shader(shader), size(size)
		   { }
		
		~BatchData() = default;

		std::array<Vector4f, Renderer::MAX_BATCH_COUNT * 4> pos;
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> texCoords;
		std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color;
		std::array<float, Renderer::MAX_BATCH_COUNT * 4> tex;
		std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices;
		std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
		uint size;
	};

	#endif

	/**
	 * @brief A Batch data storage to use to arrange data
	 * 
	 */
	struct SC_API Batch
	{
		T_Batch_Data_Array entities;
		std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
		#ifdef BATCH_TYPE_CLASS_ARRAY
		uint32_t entC;
		#endif
	};
}

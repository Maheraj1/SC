#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene/Scene.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"

namespace SC::Internal
{
	class BatchData;
	struct CameraData;
	class SC_API Renderer
	{
	public:
		static const int MAX_BATCH_COUNT = 1000;
		static const int MAX_TEXTURE_SLOT_USAGE = 10;
		
		static bool WireFrameMode;
		static bool IsFrameBuffer;

		static unsigned int VAO;
		static uint ErrorShaderID;

		static Vector2i Resolution;
		
		static void Init();
		static void Render();
		static void CleanUp();
		static void StartBatch();

		static void SetMVP(Matrix4 proj);

	private:
		static std::vector<BatchData> batchData;
		Renderer() {}
	};

	class SC_API EntityBatchArray
	{
	public:
		EntityBatchArray();
		~EntityBatchArray();

		void operator++();
		void operator--();

		std::array<Vector4f, Renderer::MAX_BATCH_COUNT * 4> pos;
		std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color;
		std::array<float, Renderer::MAX_BATCH_COUNT * 4> textureIndex;
		uint32_t entC;
	};

	/**
	 * @brief A Batch data storage to use to arrange data
	 * 
	 */
	struct SC_API Batch
	{
		EntityBatchArray entities;
		std::array<uint, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
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
						 uint size,
						 bool IsUI
						)
		  :pos(pos), texCoords(texCoords), color(color), tex(tex), indices(indices), Textures(Textures), shader(shader), size(size), IsUI(IsUI)
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
		bool IsUI;
	};
}

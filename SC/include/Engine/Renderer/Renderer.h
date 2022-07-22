#pragma once

#include "Engine/Core/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene/Scene.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"

namespace SC::Internal
{
	class BatchData;
	class Renderer
	{
	public:
		static const int MAX_BATCH_COUNT = 1000;
		static const int MAX_TEXTURE_SLOT_USAGE = 10;
		
		static bool WireFrameMode;

		static unsigned int VAO;
		static unsigned int ErrorShaderID;
		
		static void Init();
		static void Render();
		static void CleanUp();
		static void StartBatch(const std::vector<Entity>* objs);

		static void SetMVP(Matrix4 proj);
	private:
		Renderer() {}
	};

	class EntityBatchArray
	{
	public:
		EntityBatchArray();
		~EntityBatchArray();

		void operator++();
		void operator--();

		std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> pos;
		std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color;
		std::array<float, Renderer::MAX_BATCH_COUNT * 4> textureIndex;
		uint32_t entC;
	};

	class BatchTextureConnection
	{
		private:
			uint64_t entIDs[Renderer::MAX_BATCH_COUNT];
			uint lastIndex = 0;
		public:
			BatchTextureConnection(uint TexID = 0);
			~BatchTextureConnection();

			void AddEntity(uint64_t entID);

			decltype(auto) GetList()
			{
				return &entIDs;
			}

			uint& GetID() {return TexID;}

			uint TexID;
	};

	/**
	 * @brief A Batch data storage to use to arrange data
	 * 
	 */
	struct Batch
	{
		EntityBatchArray entities;
		std::array<BatchTextureConnection, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
	};

	/**
	 * @brief A Batch Data that contains raw data about the batch
	 * 
	 */
	class BatchData
	{
	public:
	
		BatchData(std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> pos,
				  std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> texCoords,
				   std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color,
					std::array<float, Renderer::MAX_BATCH_COUNT * 4> tex, 
					 std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices,
		 			  std::array<BatchTextureConnection, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures,
						uint shader,
						 uint size
						)
		  :pos(pos), texCoords(texCoords), color(color), tex(tex), indices(indices), Textures(Textures), shader(shader), size(size)
		   { }
		~BatchData() { }

		std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> pos;
		std::array<Vector2f, Renderer::MAX_BATCH_COUNT * 4> texCoords;
		std::array<ColorF, Renderer::MAX_BATCH_COUNT * 4> color;
		std::array<float, Renderer::MAX_BATCH_COUNT * 4> tex;
		std::array<uint, Renderer::MAX_BATCH_COUNT * 6> indices;
		std::array<BatchTextureConnection, Renderer::MAX_TEXTURE_SLOT_USAGE> Textures;
		uint shader;
		uint size;
	};
}

struct Vert
{
	std::array<SC::Vector2f, 4> pos;
	std::array<SC::Vector2i, 4> texCoords;
	std::array<SC::ColorF, 4> color;
	std::array<int, 4> tex;
};

struct Dat
{
	Vert dat[SC::Internal::Renderer::MAX_BATCH_COUNT];
};

#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <algorithm>

namespace SC::Internal {
	// just to make code look better
	#define BatchEnt batches[currentBatch].entities

	Vector2i Renderer::Resolution = {1280, 720};

	void Renderer::StartBatch()
	{
		std::vector<Batch> batches(1);
		batchData.clear();

		uint currentBatch = 0;
		for (Entity ent: SceneManager::GetCurrentScene().m_objs)
		{
			if (batches[currentBatch].entities.entC == MAX_BATCH_COUNT) currentBatch++;			
			if (!ent.HasComponent<SpriteRenderer>()) continue;

			SpriteRenderer& sr = ent.GetComponent<SpriteRenderer>();
			sr.PostRender();

			if (batches[currentBatch].shader != sr.shader->GetShaderID())
			{
				for (int i = 0; i < batches.size(); i++)
				{
					if (batches[i].shader == sr.shader->GetShaderID()) {
						currentBatch = i;
					} else if (batches[i].shader == 0)
					{
						batches[i].shader = sr.shader->GetShaderID();
					} else if (batches.size()-1 == currentBatch) {
						currentBatch = batches.size();
						batches.push_back(Batch());
						batches[currentBatch].shader = sr.shader->GetShaderID();
					}
				}
			}

			for (uint i = 0; i < MAX_TEXTURE_SLOT_USAGE; i++)
			{
				if (batches[currentBatch].Textures[i] == sr.texture->GetTextureID()) /* if texture list has the texture we need break from it add to binding list */
				{
					BatchEnt.textureIndex[BatchEnt.entC] = i;
					break;
				}
				else if (batches[currentBatch].Textures[i] == 0) /* if slot is empty we will use it */
				{
					batches[currentBatch].Textures[i] = sr.texture->GetTextureID();
					BatchEnt.textureIndex[BatchEnt.entC] = i;
					break;
				} else if (i == MAX_TEXTURE_SLOT_USAGE-1) /* if it is last slot and no empty slot is found create a new Batch */
				{
					currentBatch = batches.size();
					batches.push_back(Batch());
					batches[currentBatch].shader = sr.shader->GetShaderID();
					batches[currentBatch].Textures[0] = sr.texture->GetTextureID();
					BatchEnt.textureIndex[BatchEnt.entC] = i;
				}
			}
			batches[currentBatch].shader = sr.shader->GetShaderID();
			
			Matrix4 mat = ent.transform.GetModel(true);

			// Set all Vertex Positions
			BatchEnt.pos[BatchEnt.entC * 4 + 0] = (mat * Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));
			BatchEnt.pos[BatchEnt.entC * 4 + 1] = (mat * Vector4f( 1.0f, -1.0f, 0.0f, 1.0f));
			BatchEnt.pos[BatchEnt.entC * 4 + 2] = (mat * Vector4f( 1.0f,  1.0f, 0.0f, 1.0f));
			BatchEnt.pos[BatchEnt.entC * 4 + 3] = (mat * Vector4f(-1.0f,  1.0f, 0.0f, 1.0f));

			// Set all Color Values
			for (int i = 0; i <= 3; i++)
				BatchEnt.color[(BatchEnt.entC * 4) + i] = ((Vector3i)sr.color)/255.0f;

			BatchEnt.entC++;
		};

		for (int i = 0; i < batches.size(); i++) {
			std::array<uint, MAX_BATCH_COUNT * 6> ind;

			for (int v = 0; v/6 < batches[i].entities.entC; v += 6) {
				ind[v + 0] = 0 + ((v / 6 ) * 4);
				ind[v + 1] = 1 + ((v / 6 ) * 4);
				ind[v + 2] = 3 + ((v / 6 ) * 4);

				ind[v + 3] = 1 + ((v / 6 ) * 4);
				ind[v + 4] = 2 + ((v / 6 ) * 4);
				ind[v + 5] = 3 + ((v / 6 ) * 4);
			}
			
			std::array<Vector2f, MAX_BATCH_COUNT * 4> texCoords;

			int offset = 0;
			for (int i2 = 0; i2 < batches[i].entities.entC; i2++) {
				texCoords[offset + 0] = {0, 0};
				texCoords[offset + 1] = {1, 0};
				texCoords[offset + 2] = {1, 1};
				texCoords[offset + 3] = {0, 1};
				offset += 4;
			}

			batchData.emplace_back(batches[i].entities.pos,
									texCoords,
									batches[i].entities.color,
										batches[i].entities.textureIndex,
										ind,
										batches[i].Textures,
										batches[i].shader,
											batches[i].entities.entC,
											false
									);
		}

		batches.clear();

	}
}

#include "Engine/Core/Application.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/detail/type_mat3x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>

namespace SC::Internal {
	// just to make code look better
	#define BatchEnt batches[currentBatch].entities
	#define CurBatch batches[currentBatch]

	#ifdef BATCH_TYPE_CLASS_ARRAY
		#define entC_i(i) batches[i].entC
		#define _entC CurBatch.entC
		#define BATCH_ENT_FIELD(x, pos) BatchEnt[pos].x
		#define BATCH_SET_FIELD(field, value) \
			BATCH_ENT_FIELD(field, _entC)[0] = value;\
			BATCH_ENT_FIELD(field, _entC)[1] = value;\
			BATCH_ENT_FIELD(field, _entC)[2] = value;\
			BATCH_ENT_FIELD(field, _entC)[3] = value;
	#else
		#define entC_i(i) batches[i].entities.entC
		#define _entC BatchEnt.entC
		#define BATCH_ENT_FIELD(x, pos) BatchEnt.x[pos]
		#define BATCH_SET_FIELD(field, value) \
			BATCH_ENT_FIELD(field, _entC + 0) = value;\
			BATCH_ENT_FIELD(field, _entC + 1) = value;\
			BATCH_ENT_FIELD(field, _entC + 2) = value;\
			BATCH_ENT_FIELD(field, _entC + 3) = value;
	#endif
	
	#define NEXT_ENT _entC++

	std::vector<ImQuad> Renderer::quads;
	Vector2i Renderer::Resolution = {1280, 720};
	std::vector<F_Batch_Data_Array> Renderer::batchData;

	void Renderer::StartBatch()
	{
		std::vector<Batch> batches(1);
		batchData.clear();

		uint currentBatch = 0;
		for (Entity* ent: SceneManager::GetCurrentScene().m_objs)
		{
			if (_entC == MAX_BATCH_COUNT) currentBatch++;

			SpriteRenderer* sr = (SpriteRenderer*)ent->GetAvailRenderer();
			if (sr == nullptr) continue;
			
			sr->PostRender();

			if (batches[currentBatch].shader != sr->material->shader->GetShaderID())
			{
				for (int i = 0; i < batches.size(); i++)
				{
					if (batches[i].shader == sr->material->shader->GetShaderID()) {
						currentBatch = i;
					} else if (batches[i].shader == 0)
					{
						batches[i].shader = sr->material->shader->GetShaderID();
					} else if (batches.size()-1 == currentBatch) {
						currentBatch = batches.size();
						batches.push_back(Batch());
						batches[currentBatch].shader = sr->material->shader->GetShaderID();
					}
				}
			}

			for (uint i = 0; i < MAX_TEXTURE_SLOT_USAGE; i++)
			{
				if (batches[currentBatch].Textures[i] == sr->material->texture->GetTextureID()) /* if texture list has the texture we need break from it add to binding list */
				{
                    BATCH_SET_FIELD(textureIndex, i);
					break;
				}
				else if (batches[currentBatch].Textures[i] == 0) /* if slot is empty we will use it */
				{
					batches[currentBatch].Textures[i] = sr->material->texture->GetTextureID();
					BATCH_SET_FIELD(textureIndex, i);
					break;
				} else if (i == MAX_TEXTURE_SLOT_USAGE-1) /* if it is last slot and no empty slot is found create a new Batch */
				{
					currentBatch = batches.size();
					batches.push_back(Batch());
					batches[currentBatch].shader = sr->material->shader->GetShaderID();
					batches[currentBatch].Textures[0] = sr->material->texture->GetTextureID();
					BATCH_SET_FIELD(textureIndex, i);
				}
			}
			batches[currentBatch].shader = sr->material->shader->GetShaderID();
			
			Matrix4 mat = ent->transform.GetModel(true);

			// Set all Vertex Positions
			BATCH_SET_FIELD(pos, mat * Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));
			BATCH_SET_FIELD(pos, mat * Vector4f( 1.0f, -1.0f, 0.0f, 1.0f));
			BATCH_SET_FIELD(pos, mat * Vector4f( 1.0f,  1.0f, 0.0f, 1.0f));
			BATCH_SET_FIELD(pos, mat * Vector4f(-1.0f,  1.0f, 0.0f, 1.0f));

			// Set all Color Values

			Vector3 colorMat = ((Vector3i)sr->material->color)/255.0f;
			Vector3 colorBlend = ((Vector3i)sr->blendColor)/255.0f;

			BATCH_SET_FIELD(color, colorMat * colorBlend);

			NEXT_ENT;
		};

		for (int i = 0; i < batches.size(); i++) {
			#ifdef BATCH_TYPE_CLASS_ARRAY


			for (uint j = 0; j < entC_i(i); j++) {
				std::array<uint, 6> ind;

				ind[0] = 0;
				ind[1] = 1;
				ind[2] = 3;

				ind[3] = 1;
				ind[4] = 2;
				ind[5] = 3;
				
				std::array<Vector2f, 4> texCoords;

				texCoords[0] = {0, 0};
				texCoords[1] = {1, 0};
				texCoords[2] = {1, 1};
				texCoords[3] = {0, 1};

				batchData[i][j] = F_Batch_Data(BATCH_ENT_FIELD(pos, j), texCoords, BATCH_ENT_FIELD(color, j), 
						BATCH_ENT_FIELD(textureIndex, j), ind, batches[i].Textures, batches[i].shader, entC_i(i)
				);
			}

			#else
			std::array<uint, MAX_BATCH_COUNT * 6> ind;

			for (int v = 0; v/6 < _entC; v += 6) {
				ind[v + 0] = 0 + ((v / 6 ) * 4);
				ind[v + 1] = 1 + ((v / 6 ) * 4);
				ind[v + 2] = 3 + ((v / 6 ) * 4);

				ind[v + 3] = 1 + ((v / 6 ) * 4);
				ind[v + 4] = 2 + ((v / 6 ) * 4);
				ind[v + 5] = 3 + ((v / 6 ) * 4);
			}
			
			std::array<Vector2f, MAX_BATCH_COUNT * 4> texCoords;

			int offset = 0;
			for (int i2 = 0; i2 < _entC; i2++) {
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
									entC_i(i)
									);
			#endif
		}

		batches.clear();
	}

	void Renderer::RenderQuad(Vector2 pos, float rot, Vector2 scale, Color color) {
		quads.push_back({pos, rot, scale, color});
	}

}

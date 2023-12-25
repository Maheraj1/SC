#include "Engine/Core/Application.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/detail/type_mat3x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>

#define AddNewBatch quad_data.push_back({});currentBatch = &quad_data.back()
#define BATCH_DATA currentBatch->data

namespace SC::Internal {

	std::vector<BatchQuad> Renderer::quad_data;
	Vector2i Renderer::Resolution = {1280, 720};

	void Renderer::RenderQuad(ImQuad& quad, Shader* shader) {

		BatchQuad* currentBatch = nullptr;

		if (!shader) {
			
		}
		
		if (quad_data.size() == 0) {
			AddNewBatch;
			currentBatch->shader = shader;
		}
		else {
			// set shader

			for (int i = 0; i < quad_data.size(); i++) {
				if (quad_data[i].shader->uuid == shader->uuid && quad_data[i].count < MAX_BATCH_COUNT) {
					currentBatch = &quad_data[i];
				}
			}

			if (!currentBatch) {
				AddNewBatch;
				currentBatch->shader = shader;
			}
		}
		
		uint tex = 0;

		for (int i = 0; i < currentBatch->textures.size(); i++) {
			// if the texture already exists set the index of the texture
			if (currentBatch->textures[i]->uuid == quad.texture->uuid) {
				tex = i;
			}
		}

		if (!tex) {
			// There are available slots
			if (currentBatch->textureCount < MAX_TEXTURE_SLOT_USAGE) {
				currentBatch->textures[currentBatch->textureCount] = quad.texture;
				tex = currentBatch->textureCount;
				currentBatch->textureCount++;
			} 
			// No texture slot available
			else {
				AddNewBatch;
				currentBatch->shader = shader;
				currentBatch->textures[0] = quad.texture;
				tex = 0;
				currentBatch->textureCount++;
			}
		}


		int offset_vert = currentBatch->count * 4;
		
		// Texture

		// tex

		BATCH_DATA[offset_vert + 0].tex = tex;
		BATCH_DATA[offset_vert + 1].tex = tex;
		BATCH_DATA[offset_vert + 2].tex = tex;
		BATCH_DATA[offset_vert + 3].tex = tex;

		// Texture coordinates	

		BATCH_DATA[offset_vert + 0].tex_coords = {0, 0};
		BATCH_DATA[offset_vert + 1].tex_coords = {1, 0};
		BATCH_DATA[offset_vert + 2].tex_coords = {1, 1};
		BATCH_DATA[offset_vert + 3].tex_coords = {0, 1};

		// Color
		BATCH_DATA[offset_vert + 0].color = quad.color;
		BATCH_DATA[offset_vert + 1].color = quad.color;
		BATCH_DATA[offset_vert + 2].color = quad.color;
		BATCH_DATA[offset_vert + 3].color = quad.color;

		// Position

		Matrix4 mat = quad.transform.GetModel();

		BATCH_DATA[offset_vert + 0].position = 
			(Vector2f)(mat * Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));

		BATCH_DATA[offset_vert + 1].position = 
			(Vector2f)(mat * Vector4f( 1.0f, -1.0f, 0.0f, 1.0f));

		BATCH_DATA[offset_vert + 2].position = 
			(Vector2f)(mat * Vector4f( 1.0f,  1.0f, 0.0f, 1.0f));

		BATCH_DATA[offset_vert + 3].position = 
			(Vector2f)(mat * Vector4f(-1.0f,  1.0f, 0.0f, 1.0f));
	
		// Indices

		int offset_ind = currentBatch->count * 6;

		currentBatch->indices[offset_ind + 0] = 0;
		currentBatch->indices[offset_ind + 1] = 1;
		currentBatch->indices[offset_ind + 2] = 3;

		currentBatch->indices[offset_ind + 3] = 1;
		currentBatch->indices[offset_ind + 4] = 2;
		currentBatch->indices[offset_ind + 5] = 3;
	}
	
	
	void Renderer::RenderQuad(ImQuad& quad, uint64_t shader) {
		RenderQuad(quad, Resources::GetResource<Shader>(shader));
	}
}

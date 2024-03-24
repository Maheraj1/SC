#include "Engine/Core/Application.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Scene/SceneManager.h"
#include "glm/detail/type_mat3x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>
#include <limits>

#define AddNewBatch quad_data.push_back({});currentBatch = &quad_data.back()
#define AddNewBatch_L line_data.push_back({});currentBatch = &line_data.back()
#define BATCH_DATA currentBatch->data

namespace SC::Internal {

	float BatchLine::thickness = 5.0f;
	std::vector<BatchQuad> Renderer::quad_data;
	std::vector<BatchLine> Renderer::line_data;
	std::vector<BatchLineStrip> Renderer::line_strip_data;
	Vector2i Renderer::Resolution = {1280, 720};

	void Renderer::RenderQuad(ImQuad& quad, Shader* shader) {

		BatchQuad* currentBatch = nullptr;
		
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
		
		uint tex = std::numeric_limits<uint>().max();

		for (int i = 0; i < currentBatch->textures.size(); i++) {
			if (!currentBatch->textures[i]) break;
			
			// if the texture already exists set the index of the texture
			if (currentBatch->textures[i]->uuid == quad.texture->uuid) {
				tex = i;
			}
		}

		if (tex == std::numeric_limits<uint>().max()) {
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
		auto& offset_vert_0 = BATCH_DATA[offset_vert + 0];
		auto& offset_vert_1 = BATCH_DATA[offset_vert + 1];
		auto& offset_vert_2 = BATCH_DATA[offset_vert + 2];
		auto& offset_vert_3 = BATCH_DATA[offset_vert + 3];
		
		/// Texture

		// tex

		offset_vert_0.tex = tex;
		offset_vert_1.tex = tex;
		offset_vert_2.tex = tex;
		offset_vert_3.tex = tex;

		// Texture coordinates	

		offset_vert_0.tex_coords = {0, 0};
		offset_vert_1.tex_coords = {1, 0};
		offset_vert_2.tex_coords = {1, 1};
		offset_vert_3.tex_coords = {0, 1};

		// Color
		offset_vert_0.color = quad.color;
		offset_vert_1.color = quad.color;
		offset_vert_2.color = quad.color;
		offset_vert_3.color = quad.color;

		// Position

		Matrix4 mat = quad.transform.GetModel();

		offset_vert_0.position = 
			(Vector2f)(mat * Vector4f(-1.0f, -1.0f, 0.0f, 1.0f));

		offset_vert_1.position = 
			(Vector2f)(mat * Vector4f( 1.0f, -1.0f, 0.0f, 1.0f));

		offset_vert_2.position = 
			(Vector2f)(mat * Vector4f( 1.0f,  1.0f, 0.0f, 1.0f));

		offset_vert_3.position = 
			(Vector2f)(mat * Vector4f(-1.0f,  1.0f, 0.0f, 1.0f));
	
		// Indices

		int offset_ind = currentBatch->count * 6;
		int offset_ind_quad = offset_ind / 6 * 4;

		currentBatch->indices[offset_ind + 0] = 0 + offset_ind_quad;
		currentBatch->indices[offset_ind + 1] = 1 + offset_ind_quad;
		currentBatch->indices[offset_ind + 2] = 3 + offset_ind_quad;

		currentBatch->indices[offset_ind + 3] = 1 + offset_ind_quad;
		currentBatch->indices[offset_ind + 4] = 2 + offset_ind_quad;
		currentBatch->indices[offset_ind + 5] = 3 + offset_ind_quad;

		currentBatch->count++;
	}
	// TODO: Complete this
	void Renderer::RenderLine(ImLine& line, Shader* shader) {
		
		if (line.color.size() != line.vertices.size()) {
			Debug::Error("Color & Vertices vector isn't of same size", "SC::Internal::Renderer::RenderLine");
			return;
		}
		
		if (line.IsStripped) {
			BatchLineStrip* current_batch = nullptr;
			line_strip_data.push_back({});
			current_batch = &line_strip_data.back();

			current_batch->shader = shader;
			current_batch->thickness = line.thickness;

			for (int i = 0; i < line.vertices.size(); i++) {
				current_batch->data[i].position = line.vertices[i];
				current_batch->data[i].color    = line.color[i];
			}

			current_batch->count = line.vertices.size();

		} else {
			BatchLine* currentBatch = nullptr;
			if (line_data.size() == 0) {
				AddNewBatch_L;
				currentBatch->shader = shader;
			}
			else {
				// set shader

				for (int i = 0; i < line_data.size(); i++) {
					if (line_data[i].shader->uuid == shader->uuid && line_data[i].count < MAX_SINGLE_LINE_BATCH_SIZE) {
						currentBatch = &line_data[i];
					}
				}

				if (!currentBatch) {
					AddNewBatch_L;
					currentBatch->shader = shader;
				}
			}

			for (int i = 0; i < line.vertices.size(); i += 2) {

				int offset_vert = currentBatch->count * 4 + i;
				auto& offset_vert_ = BATCH_DATA[offset_vert + 0];

				offset_vert_.color = line.color[i];
				offset_vert_.position = line.vertices[i];
			}
			// currentBatch->count;
		}
	}
	
	void Renderer::RenderQuad(ImQuad& quad, uint64_t shader) {
		RenderQuad(quad, Resources::GetResource<Shader>(shader));
	}

	void Renderer::RenderLine(ImLine& line, uint64_t shader) {
		RenderLine(line, Resources::GetResource<Shader>(shader));
	}
}

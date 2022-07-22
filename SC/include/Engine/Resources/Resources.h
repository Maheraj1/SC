#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"

#include <unordered_map>

namespace SC
{
	class SC_API Resources
	{
	public:
		/**
		 * @brief Get the Shader object if shader with the name is not found it will throw a error
		 * 
		 * @param name name of shader
		 * @return Shader&
		 */
		static Shader& GetShader(const char* name);
		/**
		 * @brief Add A Shader to system
		 * 
		 * @param name name of shader
		 * @return Shader& 
		 */
		static Shader& AddShader(const char* name);
		/**
		 * @brief Get the Shader object if shader with the name is not found it will throw a error
		 * 
		 * @param name name of shader
		 * @return Shader*
		 */
		static Shader* GetShaderPtr(const char* name);
		/**
		 * @brief Add A Shader to system
		 * 
		 * @param name name of shader
		 * @return Shader& 
		 */
		static Shader* AddShaderPtr(const char *name);
		/**
		 * @brief Get the Shader object if shader with the name is not found it will throw a error
		 * 
		 * @param name name of shader
		 * @return Shader&
		 */
		static Texture& GetTexture(const char* name);
		/**
		 * @brief Add A Texture to system
		 * 
		 * @param name name of Texture
		 * @return Texture& 
		 */
		static Texture& AddTexture(const char* name, const char* fp = nullptr);
		/**
		 * @brief Get the Texture object if Texture with the name is not found it will throw a error
		 * 
		 * @param name name of Texture
		 * @return Texture*
		 */
		static Texture* GetTexturePtr(const char* name);
		/**
		 * @brief Add A Texture to system
		 * 
		 * @param name name of Texture
		 * @return Texture*
		 */
		static Texture* AddTexturePtr(const char *name, const char* fp = nullptr);

	private:
		static std::unordered_map<const char*, Shader> m_shaders;
		static std::unordered_map<const char*, Texture> m_textures;
		static unsigned int currentShader;
		static unsigned int currentTexture;

		friend class Shader;
		friend class Texture;
		friend class Application;
		friend class Internal::Renderer;
	};
}

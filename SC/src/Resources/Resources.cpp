#include "Engine/Resources/Resources.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include <utility>

namespace SC
{
	std::unordered_map<const char*, Shader> Resources::m_shaders;
	unsigned int Resources::currentShader = 0;

	std::unordered_map<const char*, Texture> Resources::m_textures;
	unsigned int Resources::currentTexture = 0;

	Shader& Resources::GetShader(const char* name)
	{
		return m_shaders.at(name);
	}

	Shader& Resources::AddShader(const char *name)
	{
		return m_shaders[name];
	}

	Shader* Resources::GetShaderPtr(const char* name)
	{
		return &GetShader(name);
	}

	Shader* Resources::AddShaderPtr(const char *name)
	{
		return &m_shaders[name];
	}

	Texture& Resources::GetTexture(const char* name)
	{
		return m_textures.at(name);
	}

	Texture& Resources::AddTexture(const char* name, const char* fp)
	{
		m_textures[name].fp = fp;
		return m_textures.at(name);
	}

	Texture* Resources::GetTexturePtr(const char* name)
	{
		return &GetTexture(name);
	}

	Texture* Resources::AddTexturePtr(const char *name, const char* fp)
	{
		return &AddTexture(name, fp);
	}
}

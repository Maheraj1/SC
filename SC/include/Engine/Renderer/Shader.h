#pragma once

#include "Engine/Math/Math.h"
#include "Engine/Core/SCObject.h"
#include "Engine/Resources/Resource.h"

namespace SC
{
	class SC_API Shader: public Resource
	{
	private:
		unsigned int m_id;
		const char* src;

		bool Valid;
	public:
		Shader(const char* src = nullptr);
		~Shader();

		unsigned int GetShaderID() const;

		void Bind() const;
		bool Compile(const char* src);
		void Delete() override;

		virtual uint64_t GetID() const override;

		// Shader variables functions

		// void SetFloat(const char* VarName, float value);
		// void SetVector2f(const char* VarName, Vector2f vec);
		// void SetVector3f(const char* VarName, Vector3f vec);
		// void SetVector4f(const char* VarName, Vector4f vec);

		// void SetInt(const char* VarName, int value);
		// void SetVector2i(const char* VarName,Vector2i vec);
		// void SetVector3i(const char* VarName,Vector3i vec);
		// void SetVector4i(const char* VarName,Vector4i vec);
	};
}

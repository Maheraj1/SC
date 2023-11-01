#pragma once

#include "Engine/ECS/IEditorObject.h"
#include "Engine/Math/Math.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Serialization/ResourceReference.h"
#include "Engine/Serialization/SerializableObject.h"

// Contains a renderer ready data required for drawing stuff

#ifdef SC_EDITOR_IMPL
#define IEO , public IEditorObject
#else
#define IEO
#endif

namespace SC {
	struct Material: public SerializableResource IEO {
		Material();
		Material(std::string fp);
		~Material();
		
		Color color;
		ResourceReference<Shader> shader;
		ResourceReference<Texture> texture;

		void Init();

		virtual uint64_t GetID() const override { return uuid; }
		virtual void Delete() override;
	
		virtual void Serialize() const override;
		virtual void DeSerialize() override;
		
		#ifdef SC_EDITOR_IMPL
		
		virtual void OnIGUI(Editor::EditorDrawData& dcmd) override;
		virtual void PostIGUI(Editor::EditorDrawData& dcmd) override;
		
		#endif
	protected:

		std::string fp;
	};
}
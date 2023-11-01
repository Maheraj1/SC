#pragma once

#include "Engine/Resources/Resource.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Scripting/ScriptInstance.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/IScript.h"
#include "Engine/ECS/IEditorObject.h"

#ifdef SC_EDITOR_IMPL
#define IEO , public IEditorObject
#else
#define IEO
#endif

namespace SC {
	class MonoCSScript: public IScript IEO {
		public:
			MonoCSScript() = default;
			MonoCSScript(std::string filepath);
			~MonoCSScript() = default;

			void Init(std::string clsName);

			template<typename... Args>
			void CallMethod(std::string name, Args... args);

			void CallMethodNoArgs(std::string name);

			virtual void CallBuiltinFunction(Scripting::BuiltinFunction func) override;

			bool operator==(UUID other);

			virtual bool IsNative() override;
			virtual uint64_t GetCID() override;
			
			#ifdef SC_EDITOR_IMPL
			virtual void OnIGUI(Editor::EditorDrawData& dcmd) override { }
			virtual void PostIGUI(Editor::EditorDrawData& dcmd) override { }
			#endif

			virtual void Serialize() const override;
			virtual void DeSerialize() override;
		private:
			Scripting::ScriptInstance runtimeScript;

	};
}
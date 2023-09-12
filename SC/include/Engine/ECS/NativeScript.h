#pragma once

#include "Engine/ECS/IScript.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Serialization/SerializableObject.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace SC {
	class NativeScript: public IScript {
	  	public:
			NativeScript() = default;
			virtual ~NativeScript() = default;

			bool IsNative() override { return true; }

			virtual void CallBuiltinFunction(Scripting::BuiltinFunction func) override;

			template<typename T>
			T* AddComponent(uint64_t cid) { return (T*)entity->AddComponent(cid); }

			template<typename T>
			T* GetComponent(uint64_t cid) { return (T*)entity->GetComponent(cid); }

			template<typename T>
			void RemoveComponent(uint64_t cid) { entity->RemoveComponent(cid); }

			virtual uint64_t GetCID() override = 0;
			#ifdef SC_EDITOR_IMPL
			virtual void OnIGUI(Editor::EditorDrawData& dcmd) override = 0;
			virtual void PostIGUI(Editor::EditorDrawData& dcmd) override = 0;
			#endif
		private:
			virtual void Awake() { }
			virtual void Start() { }
			virtual void Update() { }
			virtual void FixedUpdate() { }
			virtual void OnDestroy() { }
		friend class Entity;
	};
}
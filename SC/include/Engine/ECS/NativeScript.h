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

			virtual uint64_t GetCID() override;
		private:
			void Awake() { }
			void Start() { }
			void Update() { }
			void FixedUpdate() { }
			void OnDestroy() { }
		friend class Entity;
	};
}
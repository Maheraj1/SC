#pragma once

#include "Engine/ECS/IScript.h"
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
		private:
			void Awake() { }
			void Start() { }
			void Update() { }
			void FixedUpdate() { }
			void OnDestroy() { }
		friend class Entity;
	};
}
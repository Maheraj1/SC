#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include "Engine/Core/UUID.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Serialization/SerializableObject.h"

#include <array>
#include <functional>
#include <iostream>
#include <unordered_map>

namespace SC {
	class Entity;
	class IScript;

	namespace Scripting {
		enum class BuiltinFunction {
			None = -1,
			Awake = 0,
			Start,
			Update,
			FixedUpdate,
			OnDestroy,
		};
	}
	
	namespace Internal {

		struct Component {
			std::string qualifiedName;
			std::string TypeName;
			std::function<IScript*(void)> CreateFunc;
			uint64_t cid;
		};

		struct ComponentData {
			static std::unordered_map<uint64_t, Component> components;
			static std::unordered_map<std::string, uint64_t> TypeNameToCID;
			static std::unordered_map<std::string, uint64_t> QualifiedNameToCID;
			static uint64_t lastID;

			static void RegisterComponent(std::string name, std::string typeIdName, std::function<IScript*(void)> func);
			static void RegisterAllComponents();
		};
	}

	template<typename T>
	struct ComponentID {
		static uint64_t cid;
	};

	class IScript: public Serialization::SerializableObject {
		public:
			virtual void CallBuiltinFunction(Scripting::BuiltinFunction func) = 0;
			virtual bool IsNative() = 0;
			virtual uint64_t GetCID() = 0;
			
			Entity* entity;

		protected:
			static uint64_t cid;
		friend class SceneSerializer;
	};
}

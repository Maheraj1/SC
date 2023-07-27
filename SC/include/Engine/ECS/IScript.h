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

#define REGISTER_COMPONENT(x) ::SC::Internal::ComponentData::RegisterComponent(#x, typeid(x).name(), [](){ return new x; });\
::SC::ComponentID<x>::cid = ::SC::Internal::ComponentData::lastID;

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

			static void RegisterComponent(std::string name, std::string qualifiedName, std::function<IScript*(void)> func);
			static void RegisterAllComponents();
		};
	}

	template<typename T>
	struct ComponentID {
		static uint64_t cid;
	};

	#ifdef SC_EDITOR_IMPL
	namespace Editor { 

		enum class EditorType {
			None = -1,
			
			Int, Float,

			Vector2
		};

		struct EditorDrawCommand {
			void* data;
			std::string name;
			EditorType type;
		};
		
		struct EditorDrawData {
			void DrawFloat  (float n  , std::string name);
			void DrawInt    (int n    , std::string name);
			void DrawVector2(Vector2 n, std::string name);

			std::vector<EditorDrawCommand> data;
		};

	}
	#endif

	class IScript: public Serialization::SerializableObject {
		public:
			IScript() = default;
			virtual ~IScript() = default;
			virtual void CallBuiltinFunction(Scripting::BuiltinFunction func) = 0;
			virtual bool IsNative() = 0;
			virtual uint64_t GetCID() = 0;
			
			#ifdef SC_EDITOR_IMPL
			virtual void OnIGUI(Editor::EditorDrawData& dcmd) = 0;
			virtual void PostIGUI(Editor::EditorDrawData& dcmd) = 0;
			#endif
			
			Entity* entity;

		protected:
			static uint64_t cid;
		friend class SceneSerializer;
	};
}

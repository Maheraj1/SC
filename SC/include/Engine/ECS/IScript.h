#pragma once

#include "Engine/Core/Common.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include "Engine/Core/UUID.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Serialization/SerializableObject.h"

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#define REGISTER_COMPONENT(x) ::SC::Internal::ComponentData::RegisterComponent(#x, typeid(x).name(), [](){ return new x; });\
::SC::ComponentID<x>::cid = ::SC::Internal::ComponentData::lastID;

namespace SC {
	class Entity;
	class IScript;

	namespace Scripting {
		enum class BuiltinFunction{
			None = -1,
			Awake = 0,
			Start,
			Update,
			FixedUpdate,
			OnDestroy,
			PreRender
		};
	}
	
	namespace Internal {

		struct Component {
			// It's actual name
			std::string qualifiedName;
			// C++ typename
			std::string TypeName;
			// Function to create the script
			std::function<IScript*(void)> CreateFunc;
			// Global id of Script
			uint64_t cid;
		};

		struct ComponentData {
			static std::vector<Component> components;
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
			
			Bool,
			Int, Float, 
			Enum,

			Vector2, Color
		};

		struct EditorDrawCommand {
			// Main data
			void* data;
			
			// Name that will apper in Editor
			std::string name;

			// Type of data
			EditorType type;

			// Attached string data if any
			std::string as_data;

			// Attached int data if any
			int ai_data;

			EditorDrawCommand(void* data, std::string name, EditorType type, std::string as_data = Common::EmptyCString, int ai_data = 0);
			~EditorDrawCommand() = default;

			void Free();
		};
		
		struct EditorDrawData {
			void DrawFloat  (float n   , std::string name);
			void DrawBool   (bool  n   , std::string name);
			void DrawInt    (int n     , std::string name);
			void DrawVector2(Vector2 n , std::string name);
			void DrawColor  (Color n  , std::string name);
			
			// `enum_names` value needs to be separated by zero or `\0`
			void DrawEnum(int n, std::string enum_names, int size, std::string name);

			void Clear();

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

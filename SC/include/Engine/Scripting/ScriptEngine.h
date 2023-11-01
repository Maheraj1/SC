#pragma once

#include "Engine/Core/Buffer.h"
#include "Engine/Scripting/RegisteredScript.h"
#include "Engine/Scripting/ScriptAssembly.h"
#include <map>
#include <unordered_map>
#include <vector>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoType MonoType;
	typedef struct _MonoDomain MonoDomain;
}

namespace SC::Scripting {

	enum class EngineTypes: int {
		None = -1,
		Int32, Int16, Int64,
		Float32, Float64,
		UInt16, UInt32, UInt64,
		Char, String,
		Bool, Byte,
		Array, Map, List,
		
		Entity, Transform, Vector2,
		// Other, OtherReference
	};

	/**
	 * @brief ClassField represents the field in a class/script
	 * 
	 */
	struct ClassField {
		// For serialisation not required to set when setting field value
		// not required to be set
		EngineTypes type = EngineTypes::None;
		MonoType* mtype = nullptr;
		MonoClass* cls;
		char* value;
		uint32_t size;
		bool reference = false;
		
		// One of these two should be set
		std::string name;
		MonoClassField* clsField;
		
		// required
		MonoObject* obj;
		
		~ClassField();
	};

	class ScriptEngine {
		public:
			ScriptEngine() = delete;
			~ScriptEngine() = delete;

			static void Init(const char* projName, const char* assembliesPath);
			static void ShutDown();
			
			static void InitMono(const char* projName, const char* assembliesPath);
			static void ShutDownMono();

			static RegisteredScript* GetScript(std::string name);
			static RegisteredScript* GetScript(UUID id);

			static void* RunMethod(MonoMethod* method, void* obj, void** parms);

			// TODO
			static ClassField GetField(ClassField field);
			static void SetField(ClassField data, Buffer& FieldData);

			static void CompileAssembly(ScriptAssembly& assembly, const char* path);
		private:
			static void LoadClasses(ScriptAssembly& assembly, bool isCore = false);
		public:
			struct ScriptEngineData {
				ScriptAssembly mainAssembly;
				ScriptAssembly coreAssembly;
				MonoDomain* domain;
				MonoDomain* appDomain;

				MonoClass* EntityClass;
				MonoClass* ScriptClass;

				std::vector<RegisteredScript> scripts;
			};

			static ScriptEngineData* GetData();
		private:
			static ScriptEngineData data;
		friend class ScriptInstance;
	};
}
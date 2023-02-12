#pragma once

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
	typedef struct _MonoDomain MonoDomain;
}

namespace SC::Scripting {
	class ScriptEngine {
		public:
			ScriptEngine() = delete;
			~ScriptEngine() = delete;

			static void Init(const char* projName, const char* assembliesPath);
			static void ShutDown();
			
			static void InitMono(const char* projName, const char* assembliesPath);
			static void ShutDownMono();

		private:
			static void LoadClasses();
		private:
			struct ScriptEngineData {
				ScriptAssembly mainAssembly;
				ScriptAssembly coreAssembly;
				MonoDomain* domain;
				MonoDomain* appDomain;

				MonoClass* EntityClass;
				MonoClass* ScriptClass;
				
				std::unordered_map<std::string, MonoClass*> Scripts;
			};

			static ScriptEngineData data;
	};
}
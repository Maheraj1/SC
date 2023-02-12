#pragma once

#include "Engine/Scripting/ScriptEngine.h"
#include <unordered_map>

namespace SC::Scripting {
	class ScriptInstance {
	  public:
		ScriptInstance();
		~ScriptInstance();
		
		MonoClass* ScriptClass;
		MonoObject* instance;
		std::unordered_map<std::string, MonoClassField*> fields;
		std::unordered_map<std::string, MonoMethod*> methods;
	};
}
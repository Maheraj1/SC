#pragma once

#include "Engine/Core/UUID.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
}

namespace SC::Scripting {
	struct RegisteredScript {
		RegisteredScript()
		:id(0), name(""), cls(nullptr)
		{}
		RegisteredScript(MonoClass* cls, std::string name = "", UUID id = UUID(0)) { }
		~RegisteredScript() = default;
		
		UUID id;
		std::string name;
		MonoClass* cls;

		inline bool operator==(std::string name);
		inline bool operator==(UUID id);
	};
}
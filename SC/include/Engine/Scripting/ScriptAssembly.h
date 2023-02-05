#pragma once

extern "C" {
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}
#include <filesystem>

namespace SC::Scripting {
	class ScriptAssembly {
	  public:
		ScriptAssembly();
		~ScriptAssembly();

		void Load(const char* path, bool loadPDB = false);
		MonoAssembly* GetHandle();
		MonoImage* GetImage();

		MonoAssembly* operator()() {
			return assemblyHandle;
		}

	  private:
	  	MonoAssembly* assemblyHandle;
		MonoImage* assemblyImage;
	};
}
#include "Engine/Scripting/ScriptAssembly.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Resources/FileSystem.h"

#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"
#include "mono/metadata/mono-debug.h"
#include "mono/utils/mono-publib.h"

#include <dlfcn.h>
#include <filesystem>

namespace SC::Scripting {
	ScriptAssembly::ScriptAssembly() { }

	ScriptAssembly::~ScriptAssembly() {
		
	}
	
	void ScriptAssembly::Load(const char* path, bool loadPDB) {
		uint size = 0;
		char* data = FileSystem::ReadFileBinary(path, &size);

		MonoImageOpenStatus status;

		MonoImage* image = mono_image_open_from_data_full(data, size, 1, &status, 0);

		if (status != MONO_IMAGE_OK) {
			const char* errorMessage = mono_image_strerror(status);
			Debug::Error(errorMessage, "SC::Scripting::ScriptAssembly::Load");
		}

		assemblyHandle = mono_assembly_load_from_full(image, path, &status, 0);
		
		if (loadPDB) {
			std::filesystem::path PDBpath = path;
			PDBpath.replace_extension(".pdb");

			if (FileSystem::FileExists(PDBpath.c_str())) {
				uint size = 0;
				char* data = FileSystem::ReadFileBinary(PDBpath, &size);

				mono_debug_open_image_from_memory(image, (const unsigned char*)data, size);

				DebugLogOut out;
				out << DebugLogColor::Green << "Loaded PDB file for " << path;
				Debug::Log(out);
			
			} else {
				Debug::Error("PDB Not Found", "SC::Scripting::ScriptAssembly::PDBFileNotFound");
			}
		}

		mono_image_close(image);

		assemblyImage = mono_assembly_get_image(assemblyHandle);
	}

	MonoAssembly* ScriptAssembly::GetHandle() {
		return assemblyHandle;
	}
	

	MonoImage* ScriptAssembly::GetImage() {
		return assemblyImage;
	}
}
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scripting/ScriptAssembly.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/class.h"
#include <map>
#include <memory>

#include <unordered_map>

namespace SC::Scripting {
	
	ScriptEngine::ScriptEngineData ScriptEngine::data;

	namespace Utils {
		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				Debug::Log("%s.%s", nameSpace, name);
			}
		}
	}

	void ScriptEngine::Init(const char* projName, const char* assembliesPath) {
		InitMono(projName, assembliesPath);
	}

	void ScriptEngine::ShutDown() {
		ShutDownMono();
	}

	void ScriptEngine::LoadClasses() {

		MonoImage* image = data.coreAssembly.GetImage();

		data.EntityClass = mono_class_from_name(image, "SCEngine", "Entity");
		data.ScriptClass = mono_class_from_name(image, "SCEngine", "Script");

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			MonoClass* _class = mono_class_from_name(image, nameSpace, name);

			if (_class == data.ScriptClass) {
				continue;
			}

			if (mono_class_is_subclass_of(data.ScriptClass, _class, false)) {
				data.Scripts[(std::string)name + nameSpace] = _class;
			}
		}
	}

	void ScriptEngine::InitMono(const char* projName, const char* assembliesPath) {
		mono_set_assemblies_path(assembliesPath);
		data.domain = mono_jit_init(projName);

		// The Script Core
		data.coreAssembly.Load("Libs/SCEngine.dll", true);
		
		// The user code
		data.mainAssembly.Load("Sandbox-Script-Assembly.dll", true);

		LoadClasses();

		Utils::PrintAssemblyTypes(data.coreAssembly.GetHandle());
	}

	void ScriptEngine::ShutDownMono() {
		mono_jit_cleanup(data.domain);
	}
}
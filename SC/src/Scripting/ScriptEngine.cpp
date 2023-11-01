#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Resources/FileSystem.h"
#include "Engine/Scripting/RegisteredScript.h"
#include "Engine/Scripting/ScriptAssembly.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
// #include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include <array>
#include <map>
#include <memory>

#include <unordered_map>

namespace SC::Scripting {
	
	ScriptEngine::ScriptEngineData ScriptEngine::data;

	static std::unordered_map<std::string, EngineTypes> NameToTypes = {
		{"System.Int16", EngineTypes::Int16},
		{"System.Int32", EngineTypes::Int32},
		{"System.Int64", EngineTypes::Int64},

		{"System.UInt16", EngineTypes::UInt16},
		{"System.UInt32", EngineTypes::UInt32},
		{"System.UInt64", EngineTypes::UInt64},

		{"System.Float",  EngineTypes::Float32},
		{"System.Double", EngineTypes::Float64},

		{"System.Boolean", EngineTypes::Bool},
		{"System.Byte", EngineTypes::Byte},
		
		{"System.String", EngineTypes::String},
		{"System.Char", EngineTypes::Char},

		// {"System.Array", EngineTypes::Array},
		// {"System.Collections.Generic.List<T>", EngineTypes::List},
		// {"System.Collections.Generic.Dictionary<TKey, TValue>", EngineTypes::Map},

		{"SCEngine.Entity", EngineTypes::Entity},
		{"SCEngine.Transform", EngineTypes::Transform},

		{"SCEngine.Vector2", EngineTypes::Vector2},
	};

	// struct FieldTypes {
	// 	constexpr static const std::array<EngineTypes, 5> Reference = {
	// 		EngineTypes::String,
	// 		EngineTypes::List,
	// 		EngineTypes::Array,
	// 		EngineTypes::Map,
	// 		EngineTypes::OtherReference
	// 	};
	// 	constexpr static std::array<EngineTypes, 15> Value = {
	// 		EngineTypes::Int32, EngineTypes::Int16, EngineTypes::Int64,
	// 		EngineTypes::Float32, EngineTypes::Float64,
	// 		EngineTypes::UInt16, EngineTypes::UInt32, EngineTypes::UInt64,
	// 		EngineTypes::Char,
	// 		EngineTypes::Bool, EngineTypes::Byte,
	// 		EngineTypes::Entity, EngineTypes::Transform, EngineTypes::Vector2,
	// 		EngineTypes::Other
	// 	};
	// };

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

		EngineTypes MonoTypeToEngineType(MonoType* type) {
			std::string name = mono_type_get_name(type);

			auto it = NameToTypes.find(name);

			if (it != NameToTypes.end()) {
				return it->second;
			}

			return EngineTypes::None;
		}
	}

	void ScriptEngine::Init(const char* projName, const char* assembliesPath) {
		InitMono(projName, assembliesPath);
	}

	void ScriptEngine::ShutDown() {
		ShutDownMono();
	}

	RegisteredScript* ScriptEngine::GetScript(std::string name) {
		for (auto&& cls : data.scripts) {
			if (cls.name == name) {
				return &cls;
			}
		}
		return nullptr;
	}

	RegisteredScript* ScriptEngine::GetScript(UUID id) {
		for (auto&& cls : data.scripts) {
			if (cls.id == id) {
				return &cls;
			}
		}
		return nullptr;
	}

	ClassField ScriptEngine::GetField(ClassField field) {

		if (!field.clsField) {
			field.clsField = mono_class_get_field_from_name(field.cls, field.name.c_str());
		}
		
		// uint32_t flags = mono_field_get_flags(field.clsField);

		// if (flags & FIELD_ATTRIBUTE)

		field.mtype = mono_field_get_type(field.clsField);
		field.type = Utils::MonoTypeToEngineType(field.mtype);
		int offset = mono_field_get_offset(field.clsField);
		field.size = mono_type_size(field.mtype, &offset);
		

		field.value = (char*)malloc(field.size);
		mono_field_get_value(field.obj, field.clsField, field.value);
		field.reference = false;

		return field;
	}

	void ScriptEngine::SetField(ClassField data, Buffer& FieldData) {
		mono_field_set_value(data.obj, data.clsField, FieldData.As<char*>());
	}

	void* ScriptEngine::RunMethod(MonoMethod *method, void *obj, void **params) {
		MonoObject* exc = nullptr;
		return mono_runtime_invoke(method, obj, params, &exc);
	}

	void ScriptEngine::LoadClasses(ScriptAssembly& assembly, bool isCore) {

		MonoImage* image = assembly.GetImage();

		if (isCore) {
			data.EntityClass = mono_class_from_name(image, "SCEngine", "Entity");
			data.ScriptClass = mono_class_from_name(image, "SCEngine", "Script");
		}

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		data.scripts.reserve(numTypes);

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
				data.scripts.emplace_back(_class, (std::string)nameSpace + name, UUID());
			}
		}
	}

	void ScriptEngine::InitMono(const char* projName, const char* assembliesPath) {
		mono_set_assemblies_path(assembliesPath);
		data.domain = mono_jit_init(projName);

		// The Script Core
		data.coreAssembly.Load("Libs/SCEngine.dll", true);

		#ifdef SC_EDITOR_IMPL
		
		CompileAssembly(data.mainAssembly, "Scandium-ScriptAssembly.csproj");

		#else

		data.mainAssembly.Load("Libs/Scandium-ScriptAssembly.dll");
		
		#endif

		LoadClasses(data.coreAssembly, true);
		LoadClasses(data.mainAssembly);

		Utils::PrintAssemblyTypes(data.mainAssembly.GetHandle());

		Debug::EditorLog("Initalized Mono & C# Script system", LogLevel::Info);
	}

	void ScriptEngine::CompileAssembly(ScriptAssembly &assembly, const char* path) {

		auto out = FileSystem::RunProgram("dotnet", "build");
		Debug::OutputLog(out);

		data.mainAssembly.Load("bin/Debug/net7.0/Scandium-ScriptAssembly.dll");
	}

	void ScriptEngine::ShutDownMono() {
		//FIXME
		// mono_jit_cleanup(data.domain);
	}

	ScriptEngine::ScriptEngineData* ScriptEngine::GetData() {
		return &data;
	}

	ClassField::~ClassField() {
		if (!reference && value != nullptr) free(value);
	}
}
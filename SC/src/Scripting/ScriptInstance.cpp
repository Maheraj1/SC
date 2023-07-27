#include "Engine/Scripting/ScriptInstance.h"

#include "Engine/Core/Core.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Scripting/SerializableField.h"
#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Serialization/SerializedData.h"
#include "mono/metadata/class.h"
#include "mono/metadata/metadata.h"
#include "mono/metadata/object.h"
#include "mono/utils/mono-error.h"

#include <array>
#include <string>
#include <sys/_types/_int8_t.h>
#include <unordered_map>

#define REGISTER_FUNC(x) funcDat.x = mono_class_get_method_from_name(ScriptClass, #x, 0);
/* 
Format
```yaml
	ScriptName: <Name of the Script with namespace>

```
*/
namespace SC::Scripting {

	ScriptInstance::ScriptInstance() { 
	
	}

	ScriptInstance::~ScriptInstance() {
	
	}

	void ScriptInstance::CreateInstance(std::string clsName) {
		_name = rs->name;
		rs = Scripting::ScriptEngine::GetScript(clsName);
		ScriptClass = rs->cls;
		
		instance = mono_object_new(ScriptEngine::data.domain, ScriptClass);
		mono_runtime_object_init(instance);

		{
			MonoClassField* field;
			void* iter = NULL;

			while ((field = mono_class_get_fields(ScriptClass, &iter))) {
				
				fields[mono_field_get_name(field)] = field;
			}
		}

		{
			MonoMethod* method;
			void* iter = NULL;

			while ((method = mono_class_get_methods(ScriptClass, &iter))) {
				methods[mono_method_get_name(method)] = method;
			}
		}

		REGISTER_FUNC(Awake);
		REGISTER_FUNC(Start);
		REGISTER_FUNC(Update);
		REGISTER_FUNC(FixedUpdate);
		REGISTER_FUNC(OnDestroy);
	}

	void ScriptInstance::DestroyInstance() {
		
	}

	void ScriptInstance::CallScriptMethodNoParms(std::string name) {
		MonoObject* error;
		
		mono_runtime_invoke(methods.at(name), instance, nullptr, &error);

		if (error) {
			Debug::Error("Error in Running the Script function", "Client::"s + name);
		}
	}

	void ScriptInstance::CallBuiltinFunction(BuiltinFunction func) {
		switch (func) { 
			case BuiltinFunction::None:
				break;

			case BuiltinFunction::Awake:
				ScriptEngine::RunMethod(funcDat.Awake, instance, nullptr);
				break;

			case BuiltinFunction::Start:
				ScriptEngine::RunMethod(funcDat.Start, instance, nullptr);
				break;

			case BuiltinFunction::Update:
				ScriptEngine::RunMethod(funcDat.Update, instance, nullptr);
				break;

			case BuiltinFunction::FixedUpdate:
				ScriptEngine::RunMethod(funcDat.FixedUpdate, instance, nullptr);
				break;

			case BuiltinFunction::OnDestroy:
				ScriptEngine::RunMethod(funcDat.OnDestroy, instance, nullptr);
				break;

		}
	}

	ClassField ScriptInstance::GetField(MonoClassField* field) const {
		ClassField cfield;
		cfield.clsField = field;
		cfield.obj = instance;
		
		ScriptEngine::GetField(cfield);

		return cfield;
	}

	ClassField ScriptInstance::GetField(std::string name) const {
		ClassField cfield;
		cfield.name = name;
		cfield.obj = instance;
		
		ScriptEngine::GetField(cfield);

		return cfield;
	}

	void ScriptInstance::SetField(ClassField& field) {
		field.obj = instance;
		ScriptEngine::GetField(field);
	}

	//TODO
	//FIXME
	void ScriptInstance::Serialize() const {
		SC_ADD_PARAMETER_S(name, "ScriptName");

		std::vector<Serialization::SerializableObject*> sfields;
		
		for (auto[name, field]: fields) {
			ClassField clsField = GetField(field);
			SerializableField _field;
			_field.name = clsField.name;
																	// Not fixed
			_field.type = mono_type_get_name_full(clsField.mtype, MonoTypeNameFormat::MONO_TYPE_NAME_FORMAT_FULL_NAME);

			_field.data.Data = (uint8_t*)clsField.value;

			sfields.push_back((Serialization::SerializableObject*)&_field);
		}

		SC_ADD_PARAM<std::vector<Serialization::SerializableObject*>>(&sfields, "Fields");
	}

	SerializableField* FindField(std::vector<SerializableField*> fields, const ClassField& fieldToFind) {
		SerializableField* field = nullptr;

		for (int i = 0; i < fields.size(); i++) {
			if (fieldToFind.name == fields[i]->name) {
				field = fields[i];
				break;
			}
		}

		return field;
	}

	void ScriptInstance::DeSerialize() {
		std::vector<Serialization::SerializableObject*> _sfields;
		
		SC_GET_PARAMETER_S(_name, "ScriptName");
		SC_GET_PARAM<std::vector<Serialization::SerializableObject*>>(&_sfields, "Fields");

		std::vector<Scripting::SerializableField*>* _fields = (std::vector<Scripting::SerializableField*>*)(&_sfields);

		for (auto[name, field]: fields) {
			ClassField clsField = GetField(field);
			auto _field = FindField(*_fields, clsField);
			
			if (!_field) continue;

			ScriptEngine::SetField(clsField, _field->data);
		}
	}
}
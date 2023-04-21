#include "Engine/Scripting/ScriptInstance.h"

#include "Engine/Core/Core.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Serialization/SerializedData.h"
#include "mono/metadata/class.h"
#include "mono/metadata/metadata.h"
#include "mono/metadata/object.h"
#include "mono/utils/mono-error.h"

#include <array>
#include <string>

#define REGISTER_FUNC(x) funcDat.x = mono_class_get_method_from_name(ScriptClass, #x, 0);
/* 
Format
```yaml
	ScriptName: <Name of the Script with namespace>

```
*/
namespace SC::Scripting {

	//TODO
	//FIXME
	struct SerialisableField: public Serialization::SerializableObject {
		std::string name;
		std::string type;

		void Serialize() const override {
			SC_ADD_PARAMETER_S(name, "Name");
			SC_ADD_PARAMETER_S(type, "Type");
			// SC_ADD_PARAMETER_S()
		}
	};

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
		Serialization::SerializedData::WriteBeginList();
		
		for (auto[name, field]: fields) {
			ClassField clsField = GetField(field);
			SerialisableField _field;
			_field.name = clsField.name;
																	// Not fixed
			_field.type = mono_type_get_name_full(clsField.mtype, MonoTypeNameFormat::MONO_TYPE_NAME_FORMAT_FULL_NAME);

			SC::Serialization::SerializedData::AddValue((Serialization::SerializableObject *)&_field, name.c_str());
		}
		
		Serialization::SerializedData::WriteEndList();
	}
}
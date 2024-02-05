#pragma once

#include "Engine/Scripting/RegisteredScript.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/ECS/IScript.h"
#include "Engine/Serialization/SerializableObject.h"
#include <string>
#include <unordered_map>

namespace SC::Scripting {
	class ScriptInstance: public Serialization::SerializableObject {
	  public:

		ScriptInstance();
		~ScriptInstance();

		void CreateInstance(std::string clsName);
		void DestroyInstance();

		void CallScriptMethodNoParms(std::string name);
		void CallBuiltinFunction(BuiltinFunction func);
		
		MonoClass* ScriptClass;
		MonoObject* instance;
		
		std::unordered_map<std::string, MonoClassField*> fields;
		std::unordered_map<std::string, MonoMethod*> methods;

		void Serialize() const override;
		void DeSerialize() override;

		const std::string& name = _name;

		ClassField GetField(MonoClassField* field) const;
		ClassField GetField(std::string name) const;
		
		void SetField (ClassField& field);
	private:
		std::string _name;
		struct BuiltinFunctionsData {
			MonoMethod* Awake;
			MonoMethod* Start; 
			MonoMethod* Update; 
			MonoMethod* FixedUpdate; 
			MonoMethod* OnDestroy;
			MonoMethod* PreRender;
		};
		BuiltinFunctionsData funcDat;
		RegisteredScript* rs;
	};
}
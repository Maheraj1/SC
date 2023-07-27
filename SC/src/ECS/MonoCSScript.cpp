#include "Engine/ECS/MonoCSScript.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Scripting/ScriptInstance.h"
#include "Engine/Serialization/SerializedData.h"
#include <array>
#include <unordered_map>

using BIFunc = SC::Scripting::BuiltinFunction;

namespace SC {
	void MonoCSScript::Init(std::string clsName) { 
		runtimeScript.CreateInstance(clsName);
	}

	void MonoCSScript::CallBuiltinFunction(BIFunc func) {
		runtimeScript.CallBuiltinFunction(func);
	}

	bool MonoCSScript::IsNative() { return false; }

	void MonoCSScript::Serialize() const {
		SC_ADD_PARAM<Serialization::SerializableObject>((Serialization::SerializableObject*)&runtimeScript, "CSScript");
	}

	void MonoCSScript::DeSerialize() {
		SC_GET_PARAM<Serialization::SerializableObject>((Serialization::SerializableObject*)&runtimeScript, "CSScript");
	}

	GET_CID_IMPL(MonoCSScript);
}
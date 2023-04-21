#include "Engine/ECS/NativeScript.h"

namespace SC {
	void NativeScript::CallBuiltinFunction(Scripting::BuiltinFunction func) {
		switch (func) {			
			case Scripting::BuiltinFunction::None: break;

			case Scripting::BuiltinFunction::Awake:
				Awake();
				break;

			case Scripting::BuiltinFunction::Start:
				Start();
				break;

			case Scripting::BuiltinFunction::Update:
				Update();
				break;

			case Scripting::BuiltinFunction::FixedUpdate:
				FixedUpdate();
				break;

			case Scripting::BuiltinFunction::OnDestroy:
				OnDestroy();
				break;
		}
	}
}
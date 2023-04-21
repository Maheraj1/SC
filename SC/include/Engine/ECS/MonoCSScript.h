#pragma once

#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Scripting/ScriptInstance.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/IScript.h"

namespace SC {
		class MonoCSScript: public IScript {
		public:
			MonoCSScript() = default;
			~MonoCSScript() = default;

			void Init(std::string clsName);

			template<typename... Args>
			void CallMethod(std::string name, Args... args);

			void CallMethodNoArgs(std::string name);

			virtual void CallBuiltinFunction(Scripting::BuiltinFunction func) override;

			bool operator==(UUID other);

			virtual bool IsNative() override;
			virtual uint64_t GetCID() override;
		private:
			Scripting::ScriptInstance runtimeScript;

			virtual void Serialize() const override;
			virtual void DeSerialize() override;
	};
}
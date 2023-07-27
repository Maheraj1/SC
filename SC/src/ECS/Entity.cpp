#include "Engine/ECS/Entity.h"
#include "Engine/Core/UUID.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/MonoCSScript.h"
#include "Engine/Scripting/ScriptInstance.h"

namespace SC
{
	IScript* Entity::AddComponent(uint64_t id) {
		scripts.push_back(Internal::ComponentData::components.at(id).CreateFunc());
		scripts.back()->entity = this;
		return scripts.back();
	}

	IScript* Entity::GetComponent(uint64_t id) const {
		
		for (auto script : scripts) {
			if (script->IsNative()) {
				if (script->GetCID() == id) return script;
			}
		}

		return nullptr;
	}

	bool Entity::HasComponent(uint64_t id) const {
		
		for (auto script : scripts) {
			if (script->IsNative()) {
				if (script->GetCID() == id) return true;
			}
		}

		return false;
	}

	void Entity::RemoveComponent(uint64_t id) {
		
		for (int i = 0; i < scripts.size(); i++) {
			if (scripts[i]->IsNative()) {
				if (scripts[i]->GetCID() == id){
					scripts.erase(scripts.begin() + i);
					return;
				}
			}
		}
	}

    void Entity::Start() {
        for (auto &&script : scripts) {
            script->CallBuiltinFunction(Scripting::BuiltinFunction::Start);
        }
    }

    void Entity::Awake() {
        for (auto &&script : scripts) {
            script->CallBuiltinFunction(Scripting::BuiltinFunction::Awake);
        }
    }

    void Entity::Update() {
        for (auto &&script : scripts) {
            script->CallBuiltinFunction(Scripting::BuiltinFunction::Update);
        }
    }

    uint64_t Entity::GetUUID() const {
        return m_id;
    }

    Entity::Entity(std::string name, UUID id)
    :name(name), transform(), m_id(id) { }

    Entity::Entity(std::string name)
    :name(name), transform(), m_id() { }

	Entity::~Entity() {
		for (int i = 0; i < scripts.size(); i++) {
			delete scripts[i];
			scripts.erase(scripts.begin()+i);
		}
	}
}

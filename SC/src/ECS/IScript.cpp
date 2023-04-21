#include "Engine/ECS/IScript.h"
#include "Engine/ECS/MonoCSScript.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/ECS/Camera.h"

#define REGISTER_COMPONENT(x) ::SC::Internal::ComponentData::RegisterComponent(#x, typeid(x).name(), [](){ return new x; });\
::SC::ComponentID<x>::cid = ::SC::Internal::ComponentData::lastID;

template<typename T>
uint64_t SC::ComponentID<T>::cid;

namespace SC::Internal {
	std::unordered_map<uint64_t, Component> ComponentData::components;
	std::unordered_map<std::string, uint64_t> ComponentData::TypeNameToCID;
	std::unordered_map<std::string, uint64_t> ComponentData::QualifiedNameToCID;
	uint64_t ComponentData::lastID = 1;

	void ComponentData::RegisterComponent(std::string name, std::string typeIDName, std::function<IScript*(void)> func) {
		lastID++;
		components.try_emplace(lastID, 
			Component {
				.qualifiedName = name,
				.TypeName = typeIDName,
				.CreateFunc = func,
				.cid = lastID
			}
		);
		TypeNameToCID.try_emplace(typeIDName, lastID);
		QualifiedNameToCID.try_emplace(name, lastID);
	}

	void ComponentData::RegisterAllComponents() {
		REGISTER_COMPONENT(SC::MonoCSScript);
		REGISTER_COMPONENT(SC::Camera);
		REGISTER_COMPONENT(SC::SpriteRenderer);
		REGISTER_COMPONENT(SC::RigidBody);
		REGISTER_COMPONENT(SC::BoxCollider);
		REGISTER_COMPONENT(SC::CircleCollider);
	}
}

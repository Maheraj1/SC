#include "Engine/ECS/IScript.h"
#include "Engine/ECS/MonoCSScript.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/ECS/Camera.h"

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

#ifdef SC_EDITOR_IMPL
namespace SC::Editor { 
	
	void EditorDrawData::DrawFloat  (float n  , std::string name) { 
		data.push_back(EditorDrawCommand{ 
			.data = (void*)&n, 
			 .name = name, 
			  .type = EditorType::Float}); 
			   }

	void EditorDrawData::DrawInt    (int n    , std::string name) { 
		data.push_back(EditorDrawCommand{ 
			.data = (void*)&n, 
			 .name = name, 
			  .type = EditorType::Int}); 
			   }

	void EditorDrawData::DrawVector2(Vector2 n, std::string name) { 
		data.push_back(EditorDrawCommand{ 
			.data = (void*)&n, 
			 .name = name, 
			  .type = EditorType::Vector2}); 
			   }

}
#endif
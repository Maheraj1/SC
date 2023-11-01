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
	std::vector<Component> ComponentData::components;
	std::unordered_map<std::string, uint64_t> ComponentData::TypeNameToCID;
	std::unordered_map<std::string, uint64_t> ComponentData::QualifiedNameToCID;
	uint64_t ComponentData::lastID = 0;

	void ComponentData::RegisterComponent(std::string name, std::string typeIDName, std::function<IScript*(void)> func) {
		lastID++;
		components.push_back(
			Component {
				.qualifiedName = name,
				.TypeName = typeIDName,
				.CreateFunc = func,
				.cid = lastID
			}
		);
		TypeNameToCID.try_emplace(typeIDName, lastID-1);
		QualifiedNameToCID.try_emplace(name, lastID-1);
	}

	void ComponentData::RegisterAllComponents() {
		REGISTER_COMPONENT(SC::Camera);
		REGISTER_COMPONENT(SC::SpriteRenderer);
		REGISTER_COMPONENT(SC::RigidBody);
		REGISTER_COMPONENT(SC::BoxCollider);
		REGISTER_COMPONENT(SC::CircleCollider);
		REGISTER_COMPONENT(SC::MonoCSScript);
	}
}

#ifdef SC_EDITOR_IMPL
namespace SC::Editor { 
	
	void EditorDrawData::DrawFloat(float n, std::string name) { 
		auto dat = (float*)malloc(sizeof(float));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Float);
	}

	void EditorDrawData::DrawBool(bool n, std::string name) { 
		auto dat = (bool*)malloc(sizeof(bool));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Bool);
	}

	void EditorDrawData::DrawInt(int n, std::string name) {
		auto dat = (int*)malloc(sizeof(int));
		*dat = n;
		 
		data.emplace_back((void*)dat, name, EditorType::Int);
	}

	void EditorDrawData::DrawVector2(Vector2 n, std::string name) { 
		auto dat = (Vector2*)malloc(sizeof(Vector2));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Vector2);
	}

	void EditorDrawData::DrawColor(ColorF n, std::string name) { 
		auto dat = (ColorF*)malloc(sizeof(ColorF));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Color);
	}
	
	void EditorDrawData::DrawEnum(int n, std::string enum_names, int size, std::string name) { 
		auto dat = (int*)malloc(sizeof(int));
		*dat = n;

		data.emplace_back((void*)dat, name, EditorType::Enum, enum_names, size);
	}

	void EditorDrawData::Clear() {
		for (auto&& cmd : data) {
			cmd.Free();
		}
	}

	EditorDrawCommand::EditorDrawCommand(void* data, std::string name, EditorType type, std::string as_data, int ai_data) 
	:data(data), name(name), type(type), as_data(as_data), ai_data(ai_data)
	{ }

	void EditorDrawCommand::Free() {
		if (!data) return;
		free(data);
		data = nullptr;
	}
}
#endif
#include "Engine/ECS/IScript.h"
#include "Engine/ECS/LineRenderer.h"
#include "Engine/ECS/MonoCSScript.h"
#include "Engine/ECS/SpriteRenderer.h"
#include "Engine/Physics/BoxCollider.h"
#include "Engine/Physics/CircleCollider.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/ECS/Camera.h"
#include <algorithm>

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
		REGISTER_COMPONENT(SC::LineRenderer);
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

	void EditorDrawData::DrawVector2(Vector2f n, std::string name) { 
		auto dat = (Vector2f*)malloc(sizeof(Vector2f));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Vector2);
	}

	void EditorDrawData::DrawColor(Color n, std::string name) { 
		auto dat = (Color*)malloc(sizeof(Color));
		*dat = n;
		
		data.emplace_back((void*)dat, name, EditorType::Color);
	}

	void EditorDrawData::DrawList(void* _data, std::string name, int count, EditorType type, std::size_t size) {
		auto _size = (type == EditorType::Custom ? size : GetMemorySize(type));
		auto dat = malloc(_size * count);
		memcpy(dat, _data, _size * count);

		data.emplace_back((void*)dat, name, type, true, Common::EmptyString, count, size);
	}
	
	void EditorDrawData::DrawEnum(int n, std::string enum_names, int size, std::string name) { 
		auto dat = (int*)malloc(sizeof(int));
		*dat = n;

		data.emplace_back((void*)dat, name, EditorType::Enum, false, enum_names, size);
	}

	void EditorDrawData::Clear() {
		for (auto&& cmd : data) {
			cmd.Free();
		}
	}

	EditorDrawCommand::EditorDrawCommand(void* data, std::string name, EditorType type, bool list, std::string as_data, int ai_data, std::size_t asize_data) 
	:data(data), name(name), type(type), as_data(as_data), ai_data(ai_data), asize_data(asize_data), list(list)
	{ }

	void EditorDrawCommand::Free() {
		if (!data) return;
		free(data);
		data = nullptr;
	}

	std::size_t GetMemorySize(EditorType type) {
		switch (type) {
			case EditorType::None:
			return 0; break;

			case EditorType::Bool:
			return sizeof(bool); break; 

			case EditorType::Int:
			return sizeof(int); break;

			case EditorType::Float:
			return sizeof(float); break;

			case EditorType::Enum:
			return sizeof(int); break;

			case EditorType::Vector2:
			return sizeof(Vector2f); break;
			
			case EditorType::Color:
			return sizeof(Color); break;
			
			case EditorType::Custom:
			return 0;break;
		}
	}
}
#endif
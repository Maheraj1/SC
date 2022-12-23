#pragma once

#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/Component.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Errors.h"
#include "Engine/Core/UUID.h"
#include "Engine/ECS/ComponentData.h"

#include <array>
#include <list>
#include <vector>
#include <type_traits>
#include <exception>
#include <iostream>
#include <string.h>

namespace SC
{
	#ifdef SC_EDITOR_IMPL
	namespace Editor {
		class EditorAddon;
	}
	#endif

	class Script;
	class SC_API Entity
	{
	public:
		std::string name;
		Transform transform;

	public:
		Entity(std::string name = "New Entity");
		Entity(std::string name, UUID id);

		~Entity() { }

		uint64_t GetUUID() const;

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		T& AddComponent()
		{
			Component<T>* com = new Component<T>(
				Internal::ComponentData::components.at(Internal::ComponentData::TypeNameToComponentName[typeid(T).name()])
			);
			com->GetScript().entity = this;
			com->GetScript().transform = &transform;
			components.push_back(com);
			return com->GetScript();
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		T* AddComponentPtr()
		{
			return &AddComponent<T>();
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		T& GetComponent() const
		{
			for (int i = 0; i < components.capacity(); i++) 
			{
				Component<T>* com = dynamic_cast<Component<T>*>(components[i]);
				if (com != nullptr)
					return com->GetScript();
			}

			Debug::Error("Instance of Script Not Found", (std::string)"Entity::GetComponent<" + typeid(T).name() + ">::InstanceOfScriptNotFound");
			throw Errors::ScriptInstanceNotFound();
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		T* GetComponentPtr() const
		{
			return &GetComponent<T>();
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		T* TryGetComponent() const
		{
			for (int i = 0; i < components.capacity(); i++) 
			{
				Component<T>* com = dynamic_cast<Component<T>*>(components[i]);
				if (com != nullptr)
					return com->GetScriptPtr();
			}
			return nullptr;
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		bool HasComponent() const
		{
			for (int i = 0; i < components.capacity(); i++) 
			{
				if (dynamic_cast<Component<T>*>(components[i]) != nullptr) return true;
			}
			return false;
		}

		template<typename T>
		requires (std::is_base_of_v<Script, T>)
		void RemoveComponent()
		{
			for (int i = 0; i < components.capacity(); i++) 
			{
				Component<T>* com = dynamic_cast<Component<T>*>(components[i]);
				if (com != nullptr)
					return;
			}

			Debug::Error("Instance of Script Not Found", (std::string)"Entity::RemoveComponent<" + typeid(T).name() + ">::InstanceOfScriptNotFound");
			throw Errors::ScriptInstanceNotFound();
		}

		bool operator==(Entity& ent) {
			return m_id == ent.m_id;
		}
		
	private:
		void Start();
		void Update();
		void Awake();
		void Destroy();
		void ResetComponentEntityPtr();

		UUID m_id;

	protected:
		std::vector<IComponent*> components;

		friend class Scene;
		friend class SceneSerializer;
		friend class Application;
		#ifdef SC_EDITOR_IMPL
		friend class Editor::EditorAddon;
		#endif
	};
}
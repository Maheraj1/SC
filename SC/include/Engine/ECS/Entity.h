#pragma once

#include "Engine/Core/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/Component.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Errors.h"
#include "Engine/Core/UUID.h"

#include <vector>
#include <type_traits>
#include <exception>
#include <iostream>
#include <string.h>

namespace SC
{
	class Entity
	{
	public:
		std::string name;
		Transform transform;

	public:
		Entity(std::string name = "New Entity");

		~Entity() { }

		uint64_t GetUUID() const;

		template<typename T>
		T& AddComponent()
		{
			Component<T>* com = new Component<T>();
			com->GetScript().entity = this;
			components.push_back(com);
			return com->GetScript();
		}

		template<typename T>
		T* AddComponentPtr()
		{
			return &AddComponent<T>();
		}

		template<typename T>
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
		T* GetComponentPtr() const
		{
			return &GetComponent<T>();
		}

		template<typename T>
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
		bool HasComponent() const
		{
			for (int i = 0; i < components.capacity(); i++) 
			{
				if (dynamic_cast<Component<T>*>(components[i]) != nullptr) return true;
			}
			return false;
		}

		template<typename T>
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

		bool operator==(Entity& ent)
		{
			return m_id == ent.m_id;
		}
		
		friend class Scene;
		friend class SceneSerializer;
	private:
		void Start();
		void Update();
		void Awake();
		void Destroy();
		void ResetComponentEntityPtr();

		UUID m_id;

	protected:
		std::vector<IComponent*> components;
	};
}
#pragma once

#include "Engine/ECS/IScript.h"
#include "Engine/Math/Math.h"
#include "Engine/Debug/Debug.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Errors.h"
#include "Engine/Core/UUID.h"

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

	class MonoCSScript;
	class SC_API Entity
	{
	public:
		std::string name;
		Transform transform;

	public:
		Entity(std::string name = "New Entity");
		Entity(std::string name, UUID id);
		~Entity();

		uint64_t GetUUID() const;

		IScript* AddComponent(uint64_t cid);
		IScript* GetComponent(uint64_t cid) const;
		bool HasComponent(uint64_t cid) const;
		void RemoveComponent(uint64_t cid);

		bool operator==(Entity& ent) {
			return m_id == ent.m_id;
		}
		
	private:
		void Start();
		void Update();
		void Awake();
		void Destroy();

		UUID m_id;

	protected:
		std::vector<IScript*> scripts;

		friend class Scene;
		friend class SceneSerializer;
		friend class Application;
		#ifdef SC_EDITOR_IMPL
		friend class Editor::EditorAddon;
		#endif
	};
}
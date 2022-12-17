#include "Engine/Scene/Scene.h"
#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Debug/Timmer.h"
#include "Engine/Scene/SceneSerializer.h"

namespace SC
{
	Entity& Scene::AddEntity(std::string name) {
		m_objs.emplace_back(name);
		// this is required because the location to the object changes in std::vector::push_back() thus making he reference invalid
		for (int i = 0; i < m_objs.size();i++) {
			m_objs[i].ResetComponentEntityPtr();
		}
		return m_objs.at(m_objs.size()-1);
	}

	Entity* Scene::AddEntityPtr(std::string name) {
		return &AddEntity(name);
	}

	Entity& Scene::AddEntity(std::string name, UUID id) {
		m_objs.emplace_back(name, id);
		// this is required because the location to the object changes in std::vector::push_back() thus making he reference invalid
		for (int i = 0; i < m_objs.size();i++) {
			m_objs[i].ResetComponentEntityPtr();
		}
		return m_objs.at(m_objs.size()-1);
	}

	void Scene::Start() {
		for (Entity& objs: m_objs) {
			objs.Start();
		}
	}
	void Scene::Awake() {
		for (Entity& objs : m_objs) {
			objs.Awake();
		}
	}

	void Scene::Update() {
		for (Entity& objs : m_objs) {
			objs.Update();
		}
	}

	void Scene::DestroyEntity(Entity *ent) {
		DestroyList.push_back(ent);
	}

	void Scene::CleanFrame() {
		for (auto&& ent: DestroyList) {
			for (int i = 0; i < m_objs.size(); i++) if (m_objs[i] == *ent) {
				m_objs.erase(m_objs.begin()+i);
			}
		}
	}

	void Scene::Clear() {
		m_objs.clear();
		SC_DeSerialize(*this);
	}

	void Scene::Save() {
		SC_Serialize(*this);
	}

	void Scene::Load() {
		SC_DeSerialize(*this);
		if (!Application::PlayerLoopStarted) return;
		Awake();
		Start();
	}

	Scene::Scene(const char* FilePath):FilePath(FilePath) { }
	Scene::~Scene() { }
}

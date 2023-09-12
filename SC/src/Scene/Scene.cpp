#include "Engine/Scene/Scene.h"
#include "Engine/Core/Application.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Math/Math.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Debug/Timmer.h"
#include "Engine/Scene/SceneSerializer.h"

namespace SC
{
	Entity& Scene::AddEntity(std::string name) {
		return *m_objs.emplace_back(new Entity(name));
	}

	Entity* Scene::AddEntityPtr(std::string name) {
		return &AddEntity(name);
	}

	Entity& Scene::AddEntity(std::string name, UUID id) {
		return *m_objs.emplace_back(new Entity(name, id));
	}

	void Scene::Start() {
		for (Entity* objs: m_objs) {
			objs->Start();
		}
	}
	void Scene::Awake() {
		for (Entity* objs : m_objs) {
			objs->Awake();
		}
	}

	void Scene::Update() {
		for (Entity* objs : m_objs) {
			objs->Update();
		}
	}

	void Scene::DestroyEntity(Entity *ent) {
		DestroyList.push_back(ent);
	}

	void Scene::CleanFrame() {
		for (auto&& ent: DestroyList) {
			for (int i = 0; i < m_objs.size(); i++) if (*m_objs[i] == *ent) {
				delete m_objs[i];
				m_objs.erase(m_objs.begin()+i);
			}
		}
	}

	//TODO
	void Scene::Clear() {
		// SC_DeSerialize(*this);
	}

	void Scene::Save() {
		Application::AddStartOfFrameFunction([&](){SC_Serialize(*this);});
	}

	void Scene::SaveOnOtherPath(std::string path) {
		// Application::AddStartOfFrameFunction([&](){SC_SerializeOtherPath(*this, path);});
		SC_SerializeOtherPath(*this, path);
	}
	
	void Scene::LoadFromOtherPath(std::string path) {
		SC_DeSerializeOtherPath(*this, path);
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

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Component.h"
#include "Engine/ECS/Transform.h"

namespace SC
{
    void Entity::Start() {
        for (auto &&com : components) {
            com->_Start();
        }
    }

    void Entity::Awake() {
        for (auto &&com : components) {
            com->_Awake();
        }
    }

    void Entity::Update() {
        for (auto &&com : components) {
            com->_Update();
        }
    }

    void Entity::ResetComponentEntityPtr() {
        for (int i = 0; i < components.size(); i++) {
            components.at(i)->SetEntity(this);
        }
    }

    uint64_t Entity::GetUUID() const {
        return m_id;
    }

    void Entity::Destroy() {
        components.clear();
    }

    Entity::Entity(std::string name, UUID id)
    :name(name), transform(), m_id(id) { }

    Entity::Entity(std::string name)
    :name(name), transform(), m_id() { }
}

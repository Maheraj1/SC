#pragma once

#include "Engine/Core/Core.h"

#include "Engine/ECS/Component.h"
#include "Engine/ECS/IComponent.h"
#include "Engine/ECS/Transform.h"
#include "Engine/ECS/Entity.h"

#include <array>
#include <functional>
#include <iostream>

namespace SC
{
    namespace Internal {
        template<typename T>
        std::array<std::function<void(void)>, 4> GetScriptFunctions(T& script) 
        { 
            return script.GetFunctions();
        }
    }
	/**
     * @brief Script for logic
     */
    class Script
    {

    public:
        Entity* entity;
    public:
        Script();

        // Entity Functions

        template<typename T>
        T& AddComponent() {return entity->AddComponent<T>();}

        template<typename T>
        void RemoveComponent() {return entity->RemoveComponent<T>();}

        template<typename T>
        T& GetComponent()  {return entity->GetComponent<T>();}

        template<typename T>
        T* AddComponentPtr() {return entity->AddComponentPtr<T>();}

        template<typename T>
        T* GetComponentPtr()  {return entity->GetComponentPtr<T>();}

        // Scripting Api functions
        
        void Start()     { }
        void Awake()     { }
        void Update()    { }
        void OnDestroy() { }
    private:
        void Destroy(Entity* ent);

        void serialize() {} // TODO: serialization
        std::array<std::function<void(void)>, 4> GetFunctions();

        friend std::array<std::function<void(void)>, 4> Internal::GetScriptFunctions<Script>(Script& script);
    };
}

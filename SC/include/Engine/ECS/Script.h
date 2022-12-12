#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include "Engine/ECS/Transform.h"
#include "Engine/Serialization/SerializableObject.h"
#include "Engine/ECS/Entity.h"

#include <array>
#include <iostream>

namespace SC
{
	/**
     * @brief Script for logic
     */
    class SC_API Script: public Serialization::SerializableObject, public SCObject
    {

    public:
        Entity* entity;
        Transform* transform;
    public:
        Script();

        // Entity Functions

        template<typename T>
        inline T& AddComponent() {return entity->AddComponent<T>();}

        template<typename T>
        inline void RemoveComponent() {return entity->RemoveComponent<T>();}

        template<typename T>
        inline T& GetComponent()  {return entity->GetComponent<T>();}

        template<typename T>
        inline T* AddComponentPtr() {return entity->AddComponentPtr<T>();}

        template<typename T>
        inline T* GetComponentPtr()  {return entity->GetComponentPtr<T>();}

        template<typename T>
        inline T* TryGetComponent()  {return entity->TryGetComponent<T>();}

        // Scripting Api functions
        
        void Start()              { }
        void Awake()              { }
        void Update()             { }
        void OnDestroy()          { }
        void OnApplicationStart() { } // Runs when the application starts (i.e runtime or editor) to set default parameters
        void Destroy(Entity* ent);

        void Serial() const;
        void DeSerial();
    };
}

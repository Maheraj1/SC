#pragma once

#include "Engine/Core/Core.h"
#include "Engine/ECS/IComponent.h"

#include <iostream>
#include <unordered_map>
#include <vector>

namespace SC
{
    class Entity;

	template<typename T>
    class Component: public IComponent
    {
        private:
            T script;
            
        public:
            Component(const char* name)
            :script(T()), IComponent(name) { }

            ~Component() {
                script.OnDestroy();
             }

            virtual void _Start() override
            {
                script.Start();
            }
            
            virtual void _Update() override
            {
                script.Update();
            }
            
            virtual void _Awake() override
            {
                script.Awake();
            }

            virtual void _Serialize() override
            {
                script.Serial();
            }

            virtual void _DeSerialize() override
            {
                script.DeSerial();
            }

            virtual void SetEntity(Entity* ent) override
            {
                script.entity = ent;
            }

            T& GetScript()
            {
                return script;
            }

            T* GetScriptPtr()
            {
                return &script;
            }

            friend class Entity;
    };
}

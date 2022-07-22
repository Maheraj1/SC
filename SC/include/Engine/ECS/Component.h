#pragma once

#include "Engine/Core/Core.h"
#include "Engine/ECS/IComponent.h"

#include <functional>
#include <iostream>

namespace SC
{
    class Entity;

	template<typename T>
    class Component: public IComponent
    {
        private:
            T script;
            
        public:
            Component()
            :IComponent(typeid(T).name()), script(T()) { }

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

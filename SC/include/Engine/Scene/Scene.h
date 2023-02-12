#pragma once

#include "Engine/Core/Common.h"
#include "Engine/Math/Math.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/SpriteRenderer.h"

namespace SC::Internal { class Renderer; }

namespace SC
{
    class Camera;
    /**
     * @brief Scene class for objects and logic for mainloop
     */
    class SC_API Scene
    {
    private:
        const char* FilePath;
        std::vector<Entity*> m_objs;
        std::vector<Entity*> DestroyList;
        Camera* activeCamera;

        void Start();
        void Awake();
        void Update();
        void Clear();
    public:
        Scene(const char* filepath = Common::EmptyString);
        ~Scene();

        void Save();
        void Load();

        Entity& AddEntity(std::string name);
        Entity* AddEntityPtr(std::string name);
        Entity& AddEntity(std::string name, UUID id);

		std::vector<Entity*>& GetEntities() { return m_objs; }

        Camera* GetCurrentCamera() { return activeCamera; }

        void DestroyEntity(Entity* ent);

        void CleanFrame();
    
        friend class SceneSerializer;
        friend class SceneManager;
        friend class Application;
        friend class Internal::Renderer;
        friend class Camera;
        friend class Time;
        friend class Physics;
    };
};

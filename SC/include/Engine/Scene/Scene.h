#pragma once

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
        std::vector<Entity> m_objs;
        std::vector<Entity*> DestroyList;
        Camera* activeCamera;

        void Start();
        void Awake();
        void Update();
    public:
        Scene();
        ~Scene();

        Entity& AddEntity(std::string name);
        Entity* AddEntityPtr(std::string name);

        void DestroyEntity(Entity* ent);

        void CleanFrame();
    
        friend class SceneSerializer;
        friend class SceneManager;
        friend class Application;
        friend class Internal::Renderer;
        friend class Camera;
    };
};

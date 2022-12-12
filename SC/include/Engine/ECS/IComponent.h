#pragma once

namespace SC {
    namespace Serialization { class SerializedData; }
    
    class Entity;

	class IComponent
    {
    private:
        const char* name;
    public:
        IComponent(const char* name):name(name) { }
        virtual ~IComponent() = default;

        virtual void _Start() = 0;
        virtual void _Update() = 0;
        virtual void _Awake() = 0;

        virtual void _OnApplicationStart() = 0;

        virtual void _Serialize() = 0;
        virtual void _DeSerialize() = 0;

        virtual void SetEntity(Entity* ent) = 0;

        friend class Entity;
        friend class SceneSerializer;
    };
}

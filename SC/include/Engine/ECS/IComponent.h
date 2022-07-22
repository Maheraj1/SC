#pragma once

namespace SC {
    class Entity;

	class IComponent
    {
    private:
        const char* ScriptName;
    public:
        IComponent(const char* ScriptName):ScriptName(ScriptName) { }
        virtual ~IComponent() = default;

        virtual void _Start() = 0;
        virtual void _Update() = 0;
        virtual void _Awake() = 0;
        virtual void SetEntity(Entity* ent) = 0;

        friend class Entity;
    };
}

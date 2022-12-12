#pragma once

#include "Engine/Core/Core.h"

#include <unordered_map>
#include <functional>

namespace SC 
{
	class Entity;
	namespace Internal 
	{
        struct SC_API ComponentData
        {
            static std::vector<const char*> components;
            static std::unordered_map<std::string, int> TypeNameToComponentName;
            static std::unordered_map<std::string, void(*)(Entity*)> NameToFunc;
        };
    }
}
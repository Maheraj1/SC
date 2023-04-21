#pragma once

#include "Engine/Resources/ResourceMap.h"

#define SC_REGISTER_RESOURCE(x)   template<> std::unordered_map<std::string, x> SC::ResourceMap<x>::data = std::unordered_map<std::string, x>();\
template<>\
void SC::ResourceMap<x>::Clear()\
{\
	SC::ResourceMap<x>::data.clear();\
}
#define SC_CLEAR_RESOURCE_DATA(x) SC::ResourceMap<x>::Clear()

namespace SC
{
	class SC_API Resources
	{
	public:
		template<typename T>
		// requires (std::is_base_of_v<Resource, T>)
		inline static T* GetResource(uint64_t id)
		{
			for (auto&& [name, res] : ResourceMap<T>::data) {
				if (res.GetID() == id)
				{
					return &res;
				}
			}
			return nullptr;
		}

		template<typename T>
		// requires (std::is_base_of_v<Resource, T>)
		inline static T* GetResource(const char *name)
		{
			return &ResourceMap<T>::data.at(name);
		}

		template<typename T, typename... Args>
		// requires (std::is_base_of_v<Resource, T>)
		inline static T* AddResource(const char *name, Args ...args)
		{
			ResourceMap<T>::data.try_emplace((std::string)name, args...);
			return &ResourceMap<T>::data[name];
		}

		static void LoadFileResources(const char* fp);
	};
}
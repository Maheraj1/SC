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
			for (auto&& res : ResourceMap<T>::data) {
				if (res.GetID() == id) {
					return &res;
				}
			}
			return nullptr;
		}

		template<typename T>
		// requires (std::is_base_of_v<Resource, T>)
		inline static T* GetResource(const char *name)
		{
			for (auto&& res : ResourceMap<T>::data) {
				if (res.name == name) {
					return &res;
				}
			}
			return nullptr;
		}

		template<typename T, typename... Args>
		// requires (std::is_base_of_v<Resource, T>)
		inline static T* AddResource(const char *name, Args ...args)
		{
			auto res = ResourceMap<T>::data.emplace_back(args...);
			res.name = name;
			return &res;
		}
		template<typename T>
		static void SaveResource  (UUID uid);
		template<typename T>
		static void ReLoadResource(UUID uid);

		static void LoadFileResources(const char* fp);
		static void ReLoadFileResources(const char* fp);
	};
}
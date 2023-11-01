#pragma once

#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/SerializableResource.h"
#include "Engine/Serialization/SerializedData.h"

#define SC_REGISTER_RESOURCE(x)   template<> std::vector<x*> SC::ResourceMap<x>::data = std::vector<x*>();\
template<>\
void SC::ResourceMap<x>::Clear() {\
	for (auto&& res: SC::ResourceMap<x>::data) {\
		res->Delete();\
		delete res;\
		res = nullptr;\
	}\
	SC::ResourceMap<x>::data.clear();\
}
#define SC_CLEAR_RESOURCE_DATA(x) SC::ResourceMap<x>::Clear()

namespace SC
{
	class SC_API Resources
	{
	public:
		template<typename T>
		inline static T* GetResource(uint64_t id)
		{
			for (auto&& res : ResourceMap<T>::data) {
				if (res->GetID() == id) {
					return res;
				}
			}
			return nullptr;
		}

		template<typename T>
		inline static T* GetResource(std::string name)
		{
			for (auto&& res : ResourceMap<T>::data) {
				if (res->name == name) {
					return res;
				}
			}
			return nullptr;
		}

		template<typename T, typename... Args>
		inline static T* AddResource(std::string name, Args ...args)
		{
			T* res = ResourceMap<T>::data.emplace_back(new T(std::forward<Args>(args)...));
			res->name = name.c_str();
			return res;
		}
		template<typename T>
		inline static bool SaveResource  (UUID uid) {
			Resource* resource = nullptr;

			for (auto&& res : ResourceMap<T>::data) {
				if (res->GetID() == uid) {
					resource = res;
					break;
				}
			}

			if (!resource || !resource->IsSerializable()) return false;

			SerializableResource* res = (SerializableResource*)resource;

			Serialization::SerializedData::SetNewEmitter();
			
			res->Serialize();

			Serialization::SerializedData::SaveEmitter(res->fp);
			Serialization::SerializedData::emt = nullptr;
		}

		static void LoadFileResources(std::string fp);
		static void LoadSerializableFileResources(std::string fp);
		static void ReLoadFileResources(std::string fp);

		inline static void SetAutoSaveSerializableResource(bool autosave) { Resources::autoSave = autosave; }
		static void SaveSerializableResource(SerializableResource* serializableResource);
		static void SaveAllSerializableResource();

		static bool autoSave;
	};
}
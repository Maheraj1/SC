#pragma once

#include "Engine/Core/SCObject.h"
#include "Engine/Core/UUID.h"
#include "Engine/Resources/ResourceMap.h"
#include "Engine/Resources/Resources.h"
#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Serialization/SerializedData.h"
#include <functional>
#include <sys/_types.h>
#include <type_traits>

namespace SC
{
	/**
	 * @brief ResourceReference can be used to reference a Resource with their id
	 * 
	 * @tparam T 
	 */
	template<typename T>
	// requires (std::is_base_of_v<Resource, T>)
	class ResourceReference: public Serialization::SerializableObject
	{
		public:
			T* obj;

			ResourceReference(T* obj)
			:obj(obj)
			{ }

			inline void Serialize() const override
			{
				uint64_t uuid = obj->uuid;
				SC_ADD_PARAMETER_S(uuid, "UUID");
			}

			inline void DeSerialize() override
			{
				uint64_t uuid = 0;
				SC_GET_PARAMETER_S(uuid, "UUID");

				obj = Resources::GetResource<T>(uuid);
			}

			T* operator->()
			{
				return obj;
			}
	};
}
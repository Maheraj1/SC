#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include "SerializedData.h"

namespace SC {
	template<typename T>
	class Component;
}
namespace SC::Serialization {
	class SerializedData;
	
	class SC_API SerializableObject
	{
		public:
			SerializableObject();
		protected:

			virtual void Serialize() const { }
			virtual void DeSerialize() { }

			void _Serialize() const;
			void _DeSerialize();
		friend void SerializedData::AddValue(const SerializableObject* dat, const char* name);
	};
}

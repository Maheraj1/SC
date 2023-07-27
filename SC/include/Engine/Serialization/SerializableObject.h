#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SCObject.h"
#include "SerializedData.h"

namespace SC::Serialization {
	class SerializedData;
	
	class SC_API SerializableObject
	{
		public:
			SerializableObject();
		protected:

			// To be overloaded by user
			virtual void Serialize() const { }
			virtual void DeSerialize() { }

			// Internal functions
			void _Serialize() const;
			void _DeSerialize();
		friend void SerializedData::AddValue<SerializableObject>(const SerializableObject* dat, const char* name);
		friend void SerializedData::GetValue<SerializableObject>(SerializableObject* dat, const char* name);
		
		friend void SerializedData::AddValue<std::vector<SerializableObject*>>(const std::vector<SerializableObject*>* dat, const char* name);
		friend void SerializedData::GetValue<std::vector<SerializableObject*>>(std::vector<SerializableObject*>* dat, const char* name);
	};
}

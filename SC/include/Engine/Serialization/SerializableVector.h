#pragma once

#include "Engine/Serialization/SerializableObject.h"
#include "Engine/Serialization/SerializedData.h"
#include <cstddef>
#include <vector>

namespace SC {
	template<typename T>
	class SerializableVector: public Serialization::SerializableObject {
	  public:
		SerializableVector() = default;
		~SerializableVector() = default;
		
		std::vector<T> data;

		std::vector<T>* operator->() {
			return &data;
		}

		inline void Serialize() const override {
			std::size_t size = data.size();
			SC_ADD_PARAM(size, "Size");

			Serialization::SerializedData::AddSeq("Data");

			for (int i = 0; i < size; i++) {
				SC_ADD_PARAM<Serialization::SerializableObject>(
					(Serialization::SerializableObject*)&data[i],
						std::to_string(i).c_str(), true
				);
			}
			Serialization::SerializedData::AddSeq(nullptr, true);
		}

		inline void DeSerialize() override {
			std::size_t size;
			SC_GET_PARAM(size, "Size");

			data.resize(size);

			for (int i = 0; i < size; i++) {
				SC_GET_PARAM((SerializableObj*)&data[i], "Data", true, i);
			}
		}
	};
}
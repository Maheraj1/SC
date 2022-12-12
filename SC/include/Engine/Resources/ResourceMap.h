#pragma once

#include "Engine/Resources/Resource.h"
#include "Engine/Core/Base.h"

#include <type_traits>
#include <unordered_map>

namespace SC 
{
	template<typename T>
	requires (std::is_base_of_v<Resource, T>)
	class SC_API ResourceMap
	{
	public:
		ResourceMap() = delete;
		~ResourceMap() = delete;

		static void Clear();
		
		static std::unordered_map<std::string, T> data;
	};
}

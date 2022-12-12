#pragma once

#include "Platform.h"
#include "Engine/Core/Common.h"
#include "Engine/Debug/Debug.h"

#include <memory>

#ifdef SC_DEBUG
	#if defined(SC_PLATFORM_WINDOWS)
		#define SC_ASSERT(x) if (!(x)) __debugbreak()
		#define SC_ASSERT_S(x, s) if (!(x)) {Debug::Error(s); SC_ASSERT(x);}
	#else
		#define SC_ASSERT(x) if (!(x)) __asm__("int $3")
		#define SC_ASSERT_S(x, s) if (!(x)) {Debug::Error(s); SC_ASSERT(x);}
	#endif
#else
	#define SC_ASSERT(x)
#endif
#ifdef SC_DEBUG
	#define SC_LOG(x, y, z) SC::Debug::Log(x, __FILE__, __LINE__, y, z)
#else
	#define SC_LOG(x, y, z) SC::Debug::Log(x, y, z)
#endif


// used to convert type to it's name
#define STRINGIFY(x) #x

// use only in source files
#define SC_REGISTER_COMPONENT(x) \
SC::Internal::ComponentData::components.push_back(STRINGIFY(x));\
SC::Internal::ComponentData::TypeNameToComponentName.try_emplace(typeid(x).name(), SC::Internal::ComponentData::components.size()-1);\
SC::Internal::ComponentData::NameToFunc.try_emplace(STRINGIFY(x), [](Entity* ent) {ent->AddComponent<x>();})

namespace SC {
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
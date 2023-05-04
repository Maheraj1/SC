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
#define GET_CID_IMPL(T) \
uint64_t T::GetCID() {\
	static uint64_t cid = 0;\
\
	if (cid == 0)\
		cid = Internal::ComponentData::TypeNameToCID[typeid(T).name()];\
\
	return cid;\
}

#define GET_CID_IMPL_SRC(T) \
uint64_t GetCID() override {\
	static uint64_t cid = 0;\
\
	if (cid == 0)\
		cid = Internal::ComponentData::TypeNameToCID[typeid(T).name()];\
\
	return cid;\
}

namespace SC {
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
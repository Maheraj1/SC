#pragma once

#include "Platform.h"
#include "Engine/Core/Common.h"
#include "Engine/Debug/Debug.h"

#include <memory>

#ifdef SC_DEBUG
	#define SC_ASSERT_S(x, s) if (!(x)) {Debug::Error(s); SC_ASSERT(x);}
	#if defined(SC_PLATFORM_WINDOWS)
		#define SC_ASSERT(x) if (!(x)) __debugbreak()
	#elif defined(SC_PLATFORM_OSX)
		#if defined(__arm64__) || defined(__aarch64__)
    		#define SC_ASSERT(x) if (!(x)) __asm__("brk #0")
		#elif defined(__x86_64__) || defined(_M_X64)
			#define SC_ASSERT(x) if (!(x)) __asm__("int $3")
		#endif
	#endif
#else
	#define SC_ASSERT(x)
	#define SC_ASSERT_S(x, s)
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
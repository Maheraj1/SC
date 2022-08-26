#pragma once

#include "Platform.h"
#include <memory>

#ifdef SC_DEBUG
	#if defined(SC_USE__asm__)
		#define SC_ASSERT(x) if (!(x)) __asm__("int $3")
	#elif defined(SC_PLATFORM_WINDOWS)
		#define SC_ASSERT() if (!(x)) __debugbreak()
	#else
		#include <signal.h>
		#define SC_ASSERT(x) if (!(x)) raise(SIGTRAP)
	#endif
#else
	#define SC_ASSERT(x)
#endif
#ifdef SC_DEBUG
	#define SC_LOG(x, y, z) SC::Debug::Log(x, __FILE__, __LINE__, y, z)
#else
	#define SC_LOG(x, y, z) SC::Debug::Log(x, y, z)
#endif

#include "Engine/Debug/Debug.h"

// use only in source files
#define SC_REGISTER_COMPONENT(x) \
SC::Internal::ComponentData::components.push_back(#x);\
SC::Internal::ComponentData::NameToComponents.try_emplace(#x, SC::Internal::ComponentData::components.size()-1);\
SC::Internal::ComponentData::NameToFunc.try_emplace(#x, [](Entity* ent) {ent->AddComponent<x>();})

#pragma once

#ifdef _WIN32
	#define SC_PLATFORM_WINDOWS
	#define SC_PLATFORM_TYPE_WIN32
#elif defined(__APPLE__) && defined(__MACH__)
	#define SC_PLATFORM_OSX
	#define SC_PLATFORM_TYPE_UNIX
#elif defined(__linux__)
	#define SC_PLATFORM_LINUX
	#define SC_PLATFORM_TYPE_UNIX
#endif

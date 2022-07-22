#pragma once

#ifdef _WIN32
	#define SC_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
	#define SC_PLATFORM_OSX
#elif defined(__linux__)
	#define SC_PLATFORM_LINUX
#endif

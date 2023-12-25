#pragma once

// Core.h file for dynamic libary support

#include <string>
#ifndef NO_USING_NAMESPACE
using namespace std::string_literals;
#endif

#include "Platform.h"

#ifndef SC_API
    #ifdef SC_SHARED
        #ifdef SC_PLATFORM_WINDOWS
            #ifdef BUILD_DLL
                #define SC_API __declspec(dllexport)
            #else
                #define SC_API __declspec(dllimport)
            #endif
        #else
            #define SC_API
        #endif
    #else
        #define SC_API
    #endif
#endif

#define SC_HDR_MAX_VALUE 10

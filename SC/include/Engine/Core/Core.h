#pragma once

// Core.h file for dynamic libary support

#include <string>
using namespace std::string_literals;

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

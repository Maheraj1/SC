#include "Engine/Debug/Debug.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Base.h"
#include <cstdarg>
#include <cstdio>
#include <ostream>
#include <sstream>
#include <string>
#include <xlocale/_stdio.h>

namespace SC
{

    void Debug::Log(std::string text, DebugLogColor color, DebugLogBackGroundColor bg)
    {
        #ifdef SC_DEBUG
        std::cout << "\e[" << (int)bg << ';' << (int)color << 'm' << text.c_str() << "\e[0m" << std::endl;
        #endif
    }
	
	void Debug::Log(DebugLogOut& buffer) {
		buffer << DebugLogColor::None << DebugLogBackGroundColor::None;
		Debug::Log(buffer.str());
	}

	void Debug::LogV(const char* fmt, va_list args) {
		int size = snprintf(NULL, 0, fmt, args);
		char* buffer = new char[size + 1];

		snprintf(buffer, size, fmt, args);
	}

	void Debug::Log(const char* fmt, ...) {
		va_list list;
		va_start(list, fmt);
		Debug::LogV(fmt, list);
		va_end(list);
	}

    void Debug::Info(std::string text)
    {
        Debug::Log(text, DebugLogColor::Green);
    }

    void Debug::Info(std::string text, std::string from)
    {
        Debug::Log('(' + from + "): " + text, DebugLogColor::Green);
    }

    void Debug::Error(std::string text)
    {
        Log((std::string)"(Error): " + text.c_str(), DebugLogColor::Red);
		#ifndef SC_DONT_BREAK_ON_ERROR
		Assert(false);
		#endif
    }

    void Debug::Error(std::string text, std::string errorType)
    {
        Log('(' + errorType + "): " + text, DebugLogColor::Red);
		#ifndef SC_DONT_BREAK_ON_ERROR
		Assert(false);
		#endif
    }

    void Debug::Warning(std::string text)
    {
        Log("(Warning): " + text, DebugLogColor::Yellow);
    }

    void Debug::Assert(bool condition)
    {
        SC_ASSERT(condition);
    }
}

#include "Engine/Debug/Debug.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Base.h"

namespace SC
{
    void Debug::Log(std::string text, DebugLogColor color, DebugLogBackGroundColor bg)
    {
        #ifdef SC_DEBUG
        std::cout << "\e[" << (int)bg << ';' << (int)color << 'm' << text.c_str() << "\e[0m" << std::endl;
        #endif
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
    }

    void Debug::Error(std::string text, std::string errorType)
    {
        Log('(' + errorType + "): " + text, DebugLogColor::Red);
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

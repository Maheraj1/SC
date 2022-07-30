#include "Engine/Debug/Debug.h"
#include "Engine/Core/Math.h"
#include "Engine/Core/Base.h"

namespace SC
{
    void Debug::Log(std::string text, DebugLogColor color, DebugLogBackGroundColor bg)
    {
        std::cout << "\e[" << (int)bg << ';' << (int)color << 'm' << text.c_str() << "\e[0m" << std::endl;
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
        std::cout << "\e[" << (int)DebugLogColor::Red << 'm' << "(Error): " << text.c_str() << "\e[0m" << std::endl;
    }

    void Debug::Error(std::string text, std::string errorType)
    {
        std::cout << "\e[" << (int)DebugLogColor::Red << 'm' << '(' << errorType.c_str() << "): " << text.c_str() << "\e[0m" << std::endl;
    }

    void Debug::Warning(std::string text)
    {
        std::cout << "\e[" << (int)DebugLogColor::Yellow << 'm' << '(' << "Warning: " << ')' << text.c_str() << "\e[0m" << std::endl;
    }

    void Debug::Assert(bool condition)
    {
        SC_ASSERT(condition);
    }
}

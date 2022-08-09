#pragma once

#include <iostream>
#include <string>

namespace SC
{

    /**
     * @brief Enum with number for colored text output
     */
    enum class DebugLogColor: int
    {
        None           = 0,
        Black          = 30,
        Red            = 31,
        Green          = 32,
        Yellow         = 33,
        Blue           = 34,
        Magenta        = 35,
        Cyan           = 36,
        White          = 37,
        BrightBlack    = 90,
        BrightRed      = 91,
        BrightGreen    = 92,
        BrightYellow   = 93,
        BrightBlue     = 94,
        BrightMagenta  = 95,
        BrightCyan     = 96,
        BrightWhite    = 97,
    };
    
    /**
     * @brief Enum with number for colored background text output
     */
    enum class DebugLogBackGroundColor: int
    {
        None           = 0,
        Black          = 40,
        Red            = 41,
        Green          = 42,
        Yellow         = 43,
        Blue           = 44,
        Magenta        = 45,
        Cyan           = 46,
        White          = 47,
        BrightBlack    = 100,
        BrightRed      = 101,
        BrightGreen    = 102,
        BrightYellow   = 103,
        BrightBlue     = 104,
        BrightMagenta  = 105,
        BrightCyan     = 106,
        BrightWhite    = 107,
    };

    /**
     * @brief struct with Debugging and Logging methods
     */
    struct Debug
    {
        /**
         * @brief Method for logging text with colors
         * 
         * @param text text to be printed
         * @param color color of text (optional)
         * @param bg background color of text (optional)
         */
        static void Log(std::string text, DebugLogColor color = DebugLogColor::White, DebugLogBackGroundColor bg = DebugLogBackGroundColor::None);

        /**
         * @brief Method for logging text with colors
         * 
         * @param text text to be printed
         * @param color color of text (optional)
         * @param bg background color of text (optional)
         */
        static void Log(std::string text, const char* file, const char* line, DebugLogColor color = DebugLogColor::White, DebugLogBackGroundColor bg = DebugLogBackGroundColor::None);

        /**
         * @brief Method for logging info
         * 
         * @param text text to be logged
         */
        static void Info(std::string text);

        /**
         * @brief Method for logging info
         * 
         * @param text text to be logged
         */
        static void Info(std::string text, std::string from);

        /**
         * @brief Method for Error Loging
         * 
         * @param text Error text to be logged
         */
        static void Error(std::string text);
        
        /**
         * @brief Method for Error Loging
         * 
         * @param text Error text to be logged
         * @param errorType Error name for more debuging information
         */
        static void Error(std::string text, std::string errorType);

        /**
         * @brief Method for Loging Warning
         * 
         * @param text Warning text to be logged
         */
        static void Warning(std::string text);

        /**
         * @brief Method to Assert in Debug Build
         * 
         * @param condition 
         */
        static void Assert(bool condition = true);
    };
};

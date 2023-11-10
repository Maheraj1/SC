#pragma once

#include "Engine/Debug/Debug.h"
#include <cstddef>
#include <string>

namespace SC::Editor {
	class Console {
		struct Log {
		public:
			std::string log;
			LogLevel loglevel;
		};
	  public:
		Console() = default;
		~Console() = default;
		

		void Init();
		void Shutdown() { }

		void DotnetBuildCallback(std::string output);
		void DebugLogCallback(std::string output, LogLevel loglevel);
		
		void DrawConsole();

		std::size_t maxChars;
		std::string bufferOutput;
		std::vector<Log> bufferLog;
	};
}
#pragma once

#include <chrono>

namespace SC {
	class Timmer
	{
		private:
			const char* funcName;
			std::chrono::steady_clock::time_point start;
		public:
			Timmer(const char* FuncName);
			~Timmer();

			void Stop();
	};

	#define SC_ScopedTimmer(x) Timmer __t(x)
}

#include "Engine/Debug/Timmer.h"
#include "Engine/Debug/Debug.h"
#include <string>
#include <chrono>

namespace SC {
	Timmer::Timmer(const char* FuncName)
	:funcName(FuncName)
	{
		start = std::chrono::high_resolution_clock::now();
	}

	void Timmer::Stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		Debug::Info(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms", funcName);
	}

	Timmer::~Timmer()
	{
		Stop();
	}
}
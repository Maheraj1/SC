#pragma once

#ifdef SC_DEBUG

namespace SC::Debugger {
	namespace Internal {
		void LogMemory(long size);
		void ReduceMemory(long size);
	}
	class Memory
	{
	private:
		static long long int HeapMemory;
	public:
		Memory() = delete;
		~Memory() = delete;

		static long long int GetHeapMemoryAmount();
		friend void Internal::LogMemory(long size);
		friend void Internal::ReduceMemory(long size);
	};

}
#endif
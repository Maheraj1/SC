#ifdef SC_DEBUG
#include "Engine/Debug/MemoryTracker.h"
#include <malloc/_malloc.h>

long long int SC::Debugger::Memory::HeapMemory;

long long int SC::Debugger::Memory::GetHeapMemoryAmount()
{
	return SC::Debugger::Memory::HeapMemory;
}

void SC::Debugger::Internal::LogMemory(long size)
{
	SC::Debugger::Memory::HeapMemory += size;
}

void SC::Debugger::Internal::ReduceMemory(long size)
{
	SC::Debugger::Memory::HeapMemory -= size;
}

void* operator new(unsigned long size)
{
	void* ptr = malloc(size);
	SC::Debugger::Internal::LogMemory(size);
	return ptr;
}

void operator delete(void* ptr, unsigned long size) noexcept
{
	SC::Debugger::Internal::ReduceMemory(size);
	free(ptr);
}

void* operator new[](unsigned long size)
{
	void* ptr = malloc(size);
	SC::Debugger::Internal::LogMemory(size);
	return ptr;
}

void operator delete[](void* ptr, unsigned long size) noexcept
{
	SC::Debugger::Internal::ReduceMemory(size);
	free(ptr);
}

#endif
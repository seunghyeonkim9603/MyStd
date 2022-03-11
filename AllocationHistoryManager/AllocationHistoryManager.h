#pragma once
#include "AllocationHistoryArray.h"

class AllocationHistoryManager final
{
	friend void* operator new(const size_t size, const char* fileName, const int line);
	friend void* operator new[](const size_t size, const char* fileName, const int line);
	friend void operator delete(void* address, const char* fileName, const int line);
	friend void operator delete[](void* address, const char* fileName, const int line);
public:
	AllocationHistoryManager();
	~AllocationHistoryManager();
private:
	static AllocationHistoryManager manager;
	AllocationHistoryArray histories;
};
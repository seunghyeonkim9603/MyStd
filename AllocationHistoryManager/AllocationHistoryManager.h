#pragma once

#include "AllocationHistory.h"

class AllocationHistoryManager final
{
	friend void* operator new(const size_t size, const char* fileName, const int line);
	friend void* operator new[](const size_t size, const char* fileName, const int line);
	friend void operator delete(void* address);
	friend void operator delete[](void* address);
public:
	AllocationHistoryManager(const size_t capacity);
	~AllocationHistoryManager();
private:
	enum { LOG_FILE_LENGTH = 32 };

	char mLogFileName[LOG_FILE_LENGTH];
	AllocationHistory* mHistories;
	size_t mCapacity;
	size_t mSize;
};

void* operator new(const size_t size, const char* fileName, const int line);
void* operator new[](const size_t size, const char* fileName, const int line);
void operator delete(void* address);
void operator delete[](void* address);

#define new new(__FILE__, __LINE__)

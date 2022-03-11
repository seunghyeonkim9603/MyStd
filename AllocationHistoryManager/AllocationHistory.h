#pragma once

#include <iostream>

class AllocationHistory final
{
	friend std::ostream& operator<<(std::ostream& os, const AllocationHistory& history);
public:
	AllocationHistory(void* address, const int allocatedSize, const char* fileName, const int line);
	AllocationHistory(const AllocationHistory& other);
	~AllocationHistory() = default;

	void operator=(const AllocationHistory& other);
	bool operator==(const AllocationHistory& other) const;

private:
	enum { MAX_FILE_NAME = 256 };

	void* mAddress;
	char mFileName[MAX_FILE_NAME];
	int mAllocatedSize;
	int mLine;
};
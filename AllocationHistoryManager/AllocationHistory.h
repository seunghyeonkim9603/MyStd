#pragma once

#include <iostream>

class AllocationHistory final
{
	friend std::ostream& operator<<(std::ostream& os, const AllocationHistory& history);
public:
	AllocationHistory() = default;
	~AllocationHistory() = default;

	void operator=(const AllocationHistory& other);
public:
	enum { MAX_FILE_NAME = 256 };

	void* Address;
	char FileName[MAX_FILE_NAME];
	size_t AllocatedSize;
	int Line;
};
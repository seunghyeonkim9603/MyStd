#include "AllocationHistory.h"

#define MAX_TEXT (512)

std::ostream& operator<<(std::ostream& os, const AllocationHistory& history)
{
	char text[MAX_TEXT];

	sprintf(text, "[%p][%7d] %s : %d", history.mAddress, history.mAllocatedSize, history.mFileName, history.mLine);
	os << text;

	return os;
}

AllocationHistory::AllocationHistory(void* address, const int allocatedSize, const char* fileName, const int line)
	: mAddress(address),
	mAllocatedSize(allocatedSize),
	mLine(line)
{
	strcpy(mFileName, fileName);
}

AllocationHistory::AllocationHistory(const AllocationHistory& other)
	: AllocationHistory(other.mAddress, other.mAllocatedSize, other.mFileName, other.mLine)
{
}

void AllocationHistory::operator=(const AllocationHistory& other)
{
	mAddress = other.mAddress;
	mAllocatedSize = other.mAllocatedSize;
	mLine = other.mLine;
	strcpy(mFileName, other.mFileName);
}

bool AllocationHistory::operator==(const AllocationHistory& other) const
{
	return mAddress == other.mAddress;
}

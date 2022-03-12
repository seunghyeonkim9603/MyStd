#pragma warning(disable:4996)

#include "AllocationHistory.h"

#define MAX_TEXT (512)

std::ostream& operator<<(std::ostream& os, const AllocationHistory& history)
{
	char text[MAX_TEXT];

	sprintf(text, "[%p][%7d] %s : %d", history.Address, history.AllocatedSize, history.FileName, history.Line);
	os << text;

	return os;
}

void AllocationHistory::operator=(const AllocationHistory& other)
{
	Address = other.Address;
	AllocatedSize = other.AllocatedSize;
	Line = other.Line;
	strcpy(FileName, other.FileName);
}


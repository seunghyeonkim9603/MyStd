#include <stdlib.h>

#include "AllocationHistory.h"
#include "AllocationHistoryArray.h"

AllocationHistoryArray::AllocationHistoryArray(const size_t capacity)
	: mCapacity(capacity),
	mSize(0)
{
	mHistories = (AllocationHistory*)malloc(sizeof(AllocationHistory) * capacity);
}

AllocationHistoryArray::~AllocationHistoryArray()
{
	free(mHistories);
}

void AllocationHistoryArray::Add(const AllocationHistory& history)
{
	if (mSize == mCapacity)
	{
		void* temp;

		mCapacity *= 2;
		temp = malloc(sizeof(AllocationHistory) * mCapacity);
		memcpy(temp, mHistories, sizeof(AllocationHistory) * mSize);

		mHistories = static_cast<AllocationHistory*>(temp);
	}
	mHistories[mSize++] = history;
}

bool AllocationHistoryArray::Remove(const AllocationHistory& history)
{
	for (size_t i = 0; i < mSize; ++i)
	{
		if (mHistories[i] == history)
		{
			mHistories[i] = mHistories[--mSize];
			return true;
		}
	}
	return false;
}

bool AllocationHistoryArray::Contains(const AllocationHistory& history) const
{
	for (size_t i = 0; i < mSize; ++i)
	{
		if (mHistories[i] == history)
		{
			return true;
		}
	}
	return false;
}
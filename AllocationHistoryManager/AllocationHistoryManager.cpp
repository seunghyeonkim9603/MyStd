#pragma warning( disable: 4996 6387 6011 26495 )

#include <ctime>
#include <fstream>

#include "AllocationHistoryManager.h"

#undef new
#undef delete

static AllocationHistoryManager gManager(4096);

void operator delete(void* address, const char* fileName, const int line) { }
void operator delete[](void* address, const char* fileName, const int line) { }

void* operator new(const size_t size, const char* fileName, const int line)
{
	void* allocated = malloc(size);

	if (gManager.mSize == gManager.mCapacity)
	{
		void* temp;

		gManager.mCapacity *= 2;
		temp = malloc(sizeof(AllocationHistory) * gManager.mCapacity);
		memcpy(temp, gManager.mHistories, sizeof(AllocationHistory) * gManager.mSize);
		free(gManager.mHistories);

		gManager.mHistories = static_cast<AllocationHistory*>(temp);
	}

	AllocationHistory& history = gManager.mHistories[gManager.mSize++];
	{
		history.Address = allocated;
		history.AllocatedSize = size;
		history.Line = line;
		strcpy_s(history.FileName, AllocationHistory::MAX_FILE_NAME, fileName);
	}
	return allocated;
}

void* operator new[](const size_t size, const char* fileName, const int line)
{
	void* allocated = malloc(size);

	if (gManager.mSize == gManager.mCapacity)
	{
		void* temp;

		gManager.mCapacity *= 2;
		temp = malloc(sizeof(AllocationHistory) * gManager.mCapacity);
		memcpy(temp, gManager.mHistories, sizeof(AllocationHistory) * gManager.mSize);
		free(gManager.mHistories);

		gManager.mHistories = static_cast<AllocationHistory*>(temp);
	}

	AllocationHistory& history = gManager.mHistories[gManager.mSize++];
	{
		history.Address = allocated;
		history.AllocatedSize = size;
		history.Line = line;
		strcpy_s(history.FileName, AllocationHistory::MAX_FILE_NAME, fileName);
	}
	return allocated;
}

void operator delete(void* address)
{
	std::ofstream os;

	for (size_t i = 0; i < gManager.mSize; ++i)
	{
		AllocationHistory& history = gManager.mHistories[i];
		if (history.Address == address)
		{
			history = gManager.mHistories[--gManager.mSize];
			free(address);

			return;
		}
		if (history.Address == static_cast<char*>(address) - sizeof(int32_t))
		{
			os.open(gManager.mLogFileName, std::ios_base::out | std::ios_base::app);
			{
				os << "ARRAY : " << history << std::endl;
			}
			os.close();
			return;
		}
	}
	os.open(gManager.mLogFileName, std::ios_base::out | std::ios_base::app);
	{
		os << "NO_ALLOC : [" << address << "]" << std::endl;
	}
	os.close();
}
void operator delete[](void* address)
{
	std::ofstream os;

	for (size_t i = 0; i < gManager.mSize; ++i)
	{
		AllocationHistory& history = gManager.mHistories[i];
		if (history.Address == address)
		{
			history = gManager.mHistories[--gManager.mSize];
			free(address);

			return;
		}
	}

	os.open(gManager.mLogFileName, std::ios_base::out | std::ios_base::app);
	{
		os << "NO_ALLOC : [" << address << "]" << std::endl;
	}
	os.close();
}

AllocationHistoryManager::AllocationHistoryManager(const size_t capacity)
	:mSize(0),
	mCapacity(capacity)
{
	mHistories = (AllocationHistory*)malloc(sizeof(AllocationHistory) * capacity);

	time_t timer;
	struct tm TM;

	time(&timer);
	localtime_s(&TM, &timer);

	sprintf(gManager.mLogFileName, "Alloc_%04d%02d%02d_%02d%02d%02d.txt"
		, TM.tm_year, TM.tm_mon, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);
}

AllocationHistoryManager::~AllocationHistoryManager()
{
	std::ofstream os;

	os.open(gManager.mLogFileName, std::ios_base::out | std::ios_base::app);
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			os << "LEAK : " << mHistories[i] << std::endl;
		}
	}
	os.close();

	free(mHistories);
}
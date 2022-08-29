#include <Windows.h>
#include <process.h>
#include <memory>
#include <string>
#include <iostream>

#include "CCrashDump.h"
#include "ObjectPool.h"

#define NUM_THREADS (2)
#define POOL_SIZE (50)
#define VALID_DATA (0x7777FFFF3333DDDD)
#define LIST_SIZE (10)

struct Data
{
	long long Data1;
	long long Data2;

	Data()
	{
		Data1 = VALID_DATA;
		Data2 = 0;
	}
};

ObjectPool<Data> gPool(POOL_SIZE);

unsigned int __stdcall TestThread(void* param)
{
	Data* list[LIST_SIZE];

	std::cout << "thread start" << std::endl;

	while (true)
	{
		for (int i = 0; i < LIST_SIZE; ++i)
		{
			list[i] = gPool.GetObject();

			Data* data = list[i];

			if (data->Data1 != VALID_DATA || data->Data2 != 0)
			{
				CCrashDump::Crash();
			}
		}
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		for (int i = 0; i < LIST_SIZE; ++i)
		{
			Data* data = list[i];

			InterlockedIncrement64(&data->Data1);
			InterlockedIncrement64(&data->Data2);

			if (data->Data1 != VALID_DATA + 1 && data->Data2 != 1)
			{
				CCrashDump::Crash();
			}
		}
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);

		for (int i = 0; i < LIST_SIZE; ++i)
		{
			Data* data = list[i];

			InterlockedDecrement64(&data->Data1);
			InterlockedDecrement64(&data->Data2);

			if (data->Data1 != VALID_DATA || data->Data2 != 0)
			{
				CCrashDump::Crash();
			}
		}
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		for (int i = 0; i < LIST_SIZE; ++i)
		{
			gPool.ReleaseObject(list[i]);
		}
	}
	return 0;
}

unsigned int __stdcall TestThread2(void* param)
{
	std::cout << "thread start" << std::endl;

	while (true)
	{
		Data* data = gPool.GetObject();

		if (data->Data1 != VALID_DATA || data->Data2 != 0)
		{
			CCrashDump::Crash();
		}

		InterlockedIncrement64(&data->Data1);
		InterlockedIncrement64(&data->Data2);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		if (data->Data1 != VALID_DATA + 1 && data->Data2 != 1)
		{
			CCrashDump::Crash();
		}

		InterlockedDecrement64(&data->Data1);
		InterlockedDecrement64(&data->Data2);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		Sleep(0);
		if (data->Data1 != VALID_DATA || data->Data2 != 0)
		{
			CCrashDump::Crash();
		}

		gPool.ReleaseObject(data);

		if (50 < gPool.GetActiveCount())
		{
			CCrashDump::Crash();
		}
	}
	return 0;
}

int main(void)
{
	CCrashDump::Init();

	HANDLE hThreads[NUM_THREADS + 2];

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, &TestThread, nullptr, 0, nullptr);
	}
	hThreads[2] = (HANDLE)_beginthreadex(nullptr, 0, &TestThread2, nullptr, 0, nullptr);
	hThreads[3] = (HANDLE)_beginthreadex(nullptr, 0, &TestThread2, nullptr, 0, nullptr);

	while (true)
	{

	}
	return 0;
}
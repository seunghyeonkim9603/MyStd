#include <Windows.h>
#include <process.h>
#include <memory>
#include <string>
#include <iostream>

#include "CCrashDump.h"
#include "ObjectPool.h"
#include "Queue.h"
#include "Stack.h"

#define NUM_THREADS (4)
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
#define LOOP_COUNT (1)

Queue<int> gQueue(50);

unsigned int __stdcall QueueTest(void* param)
{
	DWORD threadID = GetCurrentThreadId();
	int temp;

	while (true)
	{
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			gQueue.Enqueue(i, threadID);
		}
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			gQueue.TryDequeue(&temp, threadID);
		}
	}
}

unsigned int __stdcall QueueTest2(void* param)
{
	DWORD threadID = GetCurrentThreadId();
	int temp;


	while (true)
	{
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			gQueue.Enqueue(i, threadID);
		}
		for (int i = 0; i < LOOP_COUNT; ++i)
		{
			gQueue.SleepDequeue(&temp, threadID);
		}
	}
}

Stack<Data*> gStack(POOL_SIZE);

unsigned int __stdcall TestThread(void* param)
{
	Data* list[LIST_SIZE];

	std::cout << "thread start" << std::endl;

	while (true)
	{
		for (int i = 0; i < LIST_SIZE; ++i)
		{
			list[i] = gStack.Pop();

			Data* data = list[i];

			if (data->Data1 != VALID_DATA || data->Data2 != 0)
			{
				CCrashDump::Crash();
			}
		}

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

		for (int i = 0; i < LIST_SIZE; ++i)
		{
			gStack.Push(list[i]);
		}
	}
	return 0;
}

unsigned int __stdcall TestThread2(void* param)
{
	std::cout << "thread start" << std::endl;

	while (true)
	{
		Data* data = gStack.Pop();

		if (data->Data1 != VALID_DATA || data->Data2 != 0)
		{
			CCrashDump::Crash();
		}

		InterlockedIncrement64(&data->Data1);
		InterlockedIncrement64(&data->Data2);

		if (data->Data1 != VALID_DATA + 1 && data->Data2 != 1)
		{
			CCrashDump::Crash();
		}

		InterlockedDecrement64(&data->Data1);
		InterlockedDecrement64(&data->Data2);

		if (data->Data1 != VALID_DATA || data->Data2 != 0)
		{
			CCrashDump::Crash();
		}

		gStack.Push(data);
	}
	return 0;
}



int main(void)
{
	CCrashDump::Init();

	HANDLE hThreads[NUM_THREADS];

	for (int i = 0; i < POOL_SIZE; ++i)
	{
		gStack.Push(new Data());
	}

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, &QueueTest, nullptr, 0, nullptr);
	}
	//hThreads[NUM_THREADS - 1] = (HANDLE)_beginthreadex(nullptr, 0, &QueueTest2, nullptr, 0, nullptr);

	WaitForMultipleObjects(NUM_THREADS, hThreads, true, INFINITE);

	return 0;
}
